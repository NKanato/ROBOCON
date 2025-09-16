/*
 * 2023 NHK KOSEN ROBOCON
 * NIT Okinawa College team B
 * Mainboard program
 * Board:Nucleo F446RE
 * Date:June 11 2023
 * written by Kanato Nino
 *
 */


//#include
#include "mbed.h"
#include "rtos.h"
#include "servo.h"
#include "MPU6050.h"

//パラメータ設定
#define NUM_LED 5
#define CAN_DATA_SIZE 8
#define CAN_WHEEL_ID 1337   //足回りへ送信するときのID
#define CAN_HARVEST_ID 1338 //収穫機構へ送信するときのID
#define CAN_JETSON_ID 1339  //Jetsonへ送信するときのID

//ピンとか
RawSerial pc(USBTX, USBRX, 115200);
RawSerial sbdbt(PC_10, PC_11, 2400);
RawSerial esp32(PA_9, PA_10, 115200);

CAN can(PA_11, PA_12);

DigitalOut led[NUM_LED] = {
    DigitalOut(PA_0),   //ロボット起動
    DigitalOut(PA_1),   //ESP32との通信状況
    DigitalOut(PA_4),   //足回りとの通信状況
    DigitalOut(PB_0),   //収穫機構との通信状況
    DigitalOut(PC_1)    //ジャイロpitch角の状態
};

MPU6050 mpu6050;


void initRobot(void);           //ロボットの初期化
void initCan(void);             //CANの初期化
void modeSwitch(void);          //手動・自動モード切替
void manualModeProcedure(void); //手動モード時の処理
void sendToWheel(void);         //足回りへの送信(手動時)
void sendToHarvest(void);       //収穫機構への送信(手動時)
void sendToPc(void);            //足回りへの送信(自動時)

void autoModeProcedure(void);   //自動モード時の処理
void sendToJetson(void);        //Jetsonにデータ送信
inline void Config(void);       //割り込み設定
inline void PS3Data(void);      //PS3データ受信


//グローバル変数
unsigned char sendData1[CAN_DATA_SIZE];
unsigned char sendData2[CAN_DATA_SIZE];
int PS3[7];
int isAutoMode = 0;
double slope = 0;
double timeTmp = 0;
double angle = 0;
int robotSpeed;

//ピン以外のインスタンス生成
Timer timer, timer_D;
CANMessage msg;

int main(void){
    int16_t acc[3];
    int16_t gyr[3];

    Config();
    initRobot();

    while(PS3[1] != 0x14) {pc.printf(" waiting\r");}//L2,R2同時押しでロボット起動}
    led[0] = 1; //ロボ起動led on!
    pc.printf("start!!\n\r");

    while(1){
        if(PS3[0] == 0x80){ //コントローラから信号が来ているときのみ実行する

            mpu6050.MPU6050_Start();
            mpu6050.readAccelData(&acc[0]); //加速度読み込み
            slope = atan2((double)acc[0],(double)acc[2]) * 57.3248408;//rad->degree,tan(x/z)
            if(pitch >= -2 && pitch <= 2){ led[4] = 1; }
            else{ led[4] = 0; }
            //accのｙ軸の値から微分で速度をもとめる。

            modeSwitch();
            pc.printf("[yaaw]%.3f, [pitch]%.3f, [roll]%.3f", yaw, pitch, roll);
            pc.printf("td: %d rd: %d",can.tderror(), can.rderror());
            pc.printf("\n\r");

            switch(isAutoMode){ //自動/手動モード別の処理
                case 0:
                    manualModeProcedure();  //手動モード処理
                    break;

                case 1:
                    autoModeProcedure();    //自動モード処理
                    break;
            }
        }
    }
}

void modeSwitch(void){
    if(PS3[2] != 0x10){
        ThisThread::sleep_for(5);
        return;
    }
    //ここに処理をかく
    if(PS3[2] == 0x10){
        isAutoMode++;
        if(isAutoMode > 1){    //isAutoModeが1の時に自動モード
            isAutoMode = 0;
        }
    }
    while(PS3[2] == 0x10){ThisThread::sleep_for(5);}//チャタリング対策
}

void manualModeProcedure(void){
    //pc.printf(" mannual mode!! \n\r");
    sendToWheel();
    sendToHarvest();  //変数が上書きされてしまうのでCM8.17
    led[4] = 0;
}

//左ジョイスティック、L2、R2の値を送信
void sendToWheel(void){
    if(pitch < 0){//ノイズがひどいので、slope->pitch 9.29
        sendData1[7]++;
        sendData1[7]=sendData1[7]<<1;
    }
    if(yaw < 0){ sendData1[7]++; }
    pc.printf("slope%.2f yaw%.2f send7:%d ", slope, yaw, sendData1[7]);

    sendData1[0] = PS3[0];
    sendData1[1] = fabs(pitch);
    sendData1[2] = PS3[1];
    sendData1[3] = PS3[3];
    sendData1[4] = PS3[4];
    sendData1[5] = PS3[2];
    sendData1[6] = abs(yaw);


    can.write(CANMessage(CAN_WHEEL_ID,&sendData1[0],CAN_DATA_SIZE)); //データサイズは電文の仕様が確定してから変える
    ThisThread::sleep_for(2);   //送信待ち2ms
    led[2] = !led[2];
    sendData1[7] = 0;
    pc.printf("sent to wheel ");
    if(can.tderror()){
        can.reset();    //エラー処理
        led[2] = 0;
    }
}

//足回りの制御をわっしーのPCに切り替えるためのフラグ
void sendToPc(void){
    sendData1[0] = 0x90;
    sendData1[1] = (unsigned char)slope;
    sendData1[2] = PS3[2];
    sendData1[3] = 0x00;
    sendData1[4] = 0x00;
    sendData1[5] = 0x00;
    sendData1[6] = abs(yaw);
    yaw < 0 ? sendData1[7] = 1 : sendData1[7] = 0;

    can.write(CANMessage(CAN_WHEEL_ID, &sendData1[0], CAN_DATA_SIZE)); //データサイズは電文の仕様が確定してから変える
    ThisThread::sleep_for(2);   //送信待ち2ms
    led[4] = !led[4];
    pc.printf("sent to wheel ");
    if(can.tderror()){
        can.reset();    //エラー処理
        led[2] = 0;
    }
}

//右ジョイスティックの値を送信
void sendToHarvest(void){
    //int status;
    sendData2[0] = PS3[0];
    sendData2[1] = PS3[5];   //左右データ
    sendData2[2] = PS3[6];   //上下データ
    sendData2[3] = PS3[2];
    can.write(CANMessage(CAN_HARVEST_ID,&sendData2[0],CAN_DATA_SIZE));
    ThisThread::sleep_for(2);   //送信待ち2ms
    led[3] = !led[3];
    pc.printf("sent to harvest ");
    if(can.tderror()){
        can.reset();
        led[3] = 0;
    }
}


void autoModeProcedure(void){
    pc.printf(" auto mode!! \n\r");
    sendToPc();
    led[2] = 0;
    led[3] = 0;
}

void sendToJetson(void){    //jetsonに割り込みデータを送信
    sendData1[0] = PS3[0];
    sendData1[1] = slope;
    sendData1[2] = PS3[1];
    sendData1[3] = PS3[2];
    can.write(CANMessage(CAN_JETSON_ID,&sendData1[0],CAN_DATA_SIZE));
    ThisThread::sleep_for(2);
    led[4] = !led[4];
    pc.printf("send to Jetson\n\r");
    if(can.tderror()){
        can.reset();
        led[4] = 0;
    }
}

void initCan(void){
    can.reset();
    can.frequency(500000);
}

void initRobot(void){
    initCan();
    mpu6050.resetMPU6050();
    mpu6050.SetUp_MPU6050();
}


//割り込み1
inline void Config(void){
    //初期設定
    sbdbt.attach(&PS3Data, Serial::RxIrq);
}

//割り込み２
inline void PS3Data(void){
    //受信割り込みのときに来るよ
    int SBDBT_Data = sbdbt.getc();
    static int bits = 0;


    if(SBDBT_Data == 128){
        bits = 0;
    }

    if (SBDBT_Data >= 0){
        PS3[bits] = SBDBT_Data;

        if (bits == 7){
            bits = 0;
        }
        else {
            bits++;
        }
    }
}
