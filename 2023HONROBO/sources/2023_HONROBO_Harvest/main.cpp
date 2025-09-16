/*
 * 2023 NHK Kosen Robocon
 * NIT Okinawa College team B
 * Harvest System Program
 * Board:Nucleo F446RE
 * Date:June 15 2023
 * written by kanato Nino
*/

#include <cmath>
#include "mbed.h"
#include "RotaryEncoder.h"

#define NUM_LED 2
#define NUM_MOTOR 2
#define NUM_LIMIT 2
#define CAN_DATA_SIZE 4
#define CAN_HARVEST_ID 1338
#define MAX_MOTOR_SPEED 0.3
#define PERIOD_MOTOR 1275

RawCAN can(PA_11, PA_12);
DigitalOut led[NUM_LED] = {
    DigitalOut(PA_0),
    DigitalOut(PA_1)
};

DigitalOut Motor_D[NUM_MOTOR] = {
    DigitalOut(PB_1),   //左右PA_5
    DigitalOut(PB_6)    //上下PA_0
};
PwmOut Motor_P[NUM_MOTOR] = {
    PwmOut(PA_8),
    PwmOut(PF_0)
};
DigitalIn limit[NUM_LIMIT] = {
    DigitalIn(PA_7),
    DigitalIn(PA_5)
};
RotaryEncoder encoder(PA_10, PA_9);

void initCan(void);         //canを初期化
void canData(void);         //canのデータを受信する
void moveHarvest(void);     //収穫機構動かす
void initHarvest(void);     //収穫機構初期化
void initLimit(void);       //リミットスイッチ初期化
void initMotor(void);       //モーター初期化
void resetPosition(void);   //元の位置に戻す


//unsigned char *Can;
unsigned char Can[CAN_DATA_SIZE];

CANMessage msg;
Timer timer;


int main() {
    int enData;
    initHarvest();

    led[0] = 0;
    while(Can[0] != 0x80){
        canData();
        printf("waiting\n\r");
    }
    led[0] = 1;

    while(1){
        canData();
        moveHarvest();
        if(Can[3] == 0x20){
            resetPosition();
        }
        int l0 = limit[0].read();
        int l1 = limit[1].read();
        printf("[lim0]%d [lim1]%d", l0, l1);
        enData= encoder.Get_Count();
        printf("id:%d error:%d %d %d %d %d [encoder]%d\n\r",msg.id, can.rderror(),Can[0],Can[1],Can[2], Can[3], enData);
    }
}

void moveHarvest(void){
    //左右方向の処理
    if(Can[1] > 64){
        printf("LEFT\n\r");
        Motor_D[0] = 1;//未定、左に動かす
        Motor_P[0] = abs(Can[1] - 64) / 64.0;
    }
    else if(Can[1] < 64){
        printf("RIGHT\n\r");
        Motor_D[0] = 0;//未定、右に動かす
        Motor_P[0] = abs(Can[1] - 64) / 64.0;
    }
    else{
        Motor_P[0] = 0;
    }

    //上下方向の処理
    if(Can[2] > 64 && limit[0]){
        printf("DOWN\n\r");
        Motor_D[1] = 1;//未定、下に動かす
        Motor_P[1] = abs(Can[2] - 64) / 64.0;
    }
    else if(Can[2] == 64){
        Motor_P[1] = 0;
    }
    else if(Can[2] < 64 && limit[1]){
        printf("UPUPUPUP\n\r");
        Motor_D[1] = 0;//未定、上に動かす
        Motor_P[1] = abs(Can[2] - 64) / 64.0;   //UPだけできない不具合がでたので、UPの処理をelseに入れた
    }
    else{
        Motor_D[1] = 0;//未定、上に動かす
        Motor_P[1] = 0;//9.12追加
    }
}

void resetPosition(void){
    int count;

    while(true){
        printf("Reset Position\n\r");
        count = encoder.Get_Count();
        if(count > 5){  //cwで正の値になる
            Motor_D[0] = 1;//right
            Motor_P[0] = MAX_MOTOR_SPEED;
        }
        else if(count < -5){ //ccwで負の値になる
            Motor_D[0] = 0;//left
            Motor_P[0] = MAX_MOTOR_SPEED;
        }
        else{
            break;
        }
    }
}

void initHarvest(void){
    initCan();
    initLimit();
    initMotor();
}

void initCan(void){
    can.reset();
    can.frequency(500000);
    can.filter(CAN_HARVEST_ID, 0xfff, CANStandard, 0);
}

void initLimit(void){
    for(int i = 0; i < NUM_LIMIT; i++){
        limit[i].mode(PullUp);
    }
}

void initMotor(void){
    for(int i = 0; i < NUM_MOTOR; i++){
        Motor_P[i].period_us(PERIOD_MOTOR);
    }
}

void canData(void){
    //int i;
    can.read(msg);
    if(can.rderror() != 0){
        can.reset();
    }
    if(msg.id == CAN_HARVEST_ID){
        Can[0] = msg.data[0];
        Can[1] = msg.data[1];
        Can[2] = msg.data[2];
        Can[3] = msg.data[3];
    }
    else{
    }
    led[1] = !led[1];
}
