/*
 * 2023 NHK KOSEN ROBOCON
 * Board        : Nucleo F446RE
 * OS           : mbed-os5.15.7
 * Created Date : May first 2023
 * Modified Date: August 11 2023
 * Author       : Kanato Nino@NIT Okinawa College
 *
 */

//ライブラリたち
#include <cmath>
#include <cstdint>
#include "mbed.h"
#include "RawCAN.h"
#include "RotaryEncoder.h"
#include "rtos.h"
#include "servo.h"
#include "ultrasonic.h"

//パラメータ設定
#define MAX_OMNI_SPEED 0.8
#define MIN_OMNI_SPEED 0.2
#define MAX_ROLL_SPEED 0.4
#define MIN_ROLL_SPEED 0.15
#define MAX_CATERPILLAR_SPEED 0.5
#define MIN_CATERPILLAR_SPEED 0.3
#define MAX_UPDOWN_SPEED 0.9
#define MIN_UPDOWN_SPEED 0.3
#define SWITCH_DISTANCE 80     //cm
#define MAX_HARVEST_SPEED 0.5
#define MAX_PROP_SPEED 0.7
#define MAX_RAIL_SPEED 1.0
#define MIN_RAIL_SPEED 0.8
#define ACCEL_TIME 5
#define RATE_ADJUST 0.01
#define END_ANGLE 13
#define CATERPILLAR_BREAK_TIME 3.0  //sec

#define NUM_OMNI 4
#define NUM_UPDOWN 2
#define NUM_LIMIT 4
#define NUM_SONIC_SENSOR 2
#define NUM_LED 3

#define PERIOD_OMNI 1275
#define PERIOD_UPDOWN 1275
#define PERIOD_CATERPILLAR 1275

#define CAN_SIZE 8
#define CAN_WHEEL_ID 1337
#define CAN_JETSON_ID 1339

#define mC 261.626
#define PI 3.1415926535


//ピン
DigitalOut omni_D[NUM_OMNI] = {
    DigitalOut(PC_5),
    DigitalOut(PA_0),
    DigitalOut(PA_1),
    DigitalOut(PA_7)
};

PwmOut omni_P[NUM_OMNI] = {
    PwmOut(PC_6),
    PwmOut(PC_9),
    PwmOut(PC_8),
    PwmOut(PC_7)
};

DigitalOut updown_D[NUM_UPDOWN] = {
    DigitalOut(PB_1),
    DigitalOut(PB_2)
};

PwmOut updown_P[NUM_UPDOWN] = {
    PwmOut(PA_8),
    PwmOut(PA_9)

};

DigitalOut caterpillar_D(PC_4);
PwmOut caterpillar_P(PB_6);


DigitalOut led[NUM_LED] = {
    DigitalOut(PA_4),   //起動
    DigitalOut(PB_0),   //jetsonとの通信ステータス
    DigitalOut(PC_1)    //canの受信ステータス
};

//リミットスイッチ
DigitalIn limit[NUM_LIMIT] = {
    DigitalIn(PB_15),
    DigitalIn(PB_14),
    DigitalIn(PB_5),
    DigitalIn(PB_4)
};

//エンコーダ
RotaryEncoder encoder_X(PC_0, PC_3);    //encoder1
RotaryEncoder encoder_Y(PA_6, PB_13);   //encoder2

//超音波センサー
Ultrasonic ultrasonic1(PB_10, PA_10);

//通信
RawSerial pc(USBTX,USBRX,115200);
RawSerial jetson(PC_10, PC_11, 9600);
RawCAN can(PB_8, PB_9);

//増設のモーター
DigitalOut rail_D(PA_15);
PwmOut rail_P(PB_7);
DigitalOut prop_D(PA_12);
PwmOut prop_P(PA_11);


void initWheels(void);                  //全体の初期化を行う
void initCan(void);                     //CAN初期化
inline void Config(void);               //割り込み設定
inline void JetsonData(void);           //Jetson(PC)の受信関数
void canData(void);                     //CANの受信用関数
inline void stopAllWheels(void);        //足回りを止める
inline void stopOmni(void);             //オムニを止める
int readDistance(void);                 //距離を測る
void shouldSwitch(void);                //上下するかどうか？
void caterpillarMove(int direction);    //キャタピラ動かす
void caterpillarUp(void);               //キャタピラ上昇
void caterpillarDown(void);             //キャタピラ下降
void omniAcceleration(void);            //オムニ加速
void omniSlowDown(void);                //オムニ減速

void omniMovedByJetson(void);           //オムニ自動用



//新しいオムニのコード
void omniGyro(void);
void accelControl(double *targetSpeed, int mode);
bool isPressed(unsigned char data, int bits);
double maxNum(double *num);

//グローバル変数
unsigned char Can[CAN_SIZE];
int16_t yaw = 0;
int slope = 0;
int JETSON[8];
double CURRENT_OMNI_SPEED;
double currentOmniSpeed[NUM_OMNI];
double motor[NUM_OMNI];
double Angle, Radial;
double Rotate;
int directionTmp = 0;
bool isOmniLocked = false;
bool isCaterpillarLocked = true;
double caterpiBreakTime = 0;

double omniSpeedTmp[NUM_OMNI];//Gyroomniの先頭から切り取り
uint8_t forwardTmp;
int angleStatus=0;

//その他
Timer timer,timer_w,timer_r;
Ticker ticker;          //タイマ割り込み
Thread thread;
CANMessage msg;


//---------
void backToPosition(double yaw){
    int i;
    while(true){
        if(yaw > 0){
            for(i = 0; i < 4; i++){
                omni_D[i] = 1;
                omni_P[i] = MIN_OMNI_SPEED;
                pc.printf("[RIGHT]");

            }
            break;
        }
        else if(yaw < 0){
            for(i = 0; i < 4; i++){
                omni_D[i] = 0;
                omni_P[i] = MIN_OMNI_SPEED;
                pc.printf("[LEFT]");
            }
            break;
        }
        else{
            for(i = 0; i < 4; i++){
                omni_P[i] = 0;
                pc.printf("[NONE]");
            }
            break;
        }
        pc.printf("\n\r");
    }
}
//--------


//ロープ超えるためのレール動かす
void moveRail(void){
    int i;
    if(Can[2] == 0x01){
        rail_D = 0;
        rail_P = MAX_RAIL_SPEED;
        pc.printf("up rail ");
    }
    else if(Can[5] == 0x20){
        rail_D = 1;
        rail_P = MIN_RAIL_SPEED;
        pc.printf("down rail ");
    }
    else{
        rail_P = 0;
    }
}
//--------


int main(void){
    int i,j;    //カウント用変数
    initWheels();
    int sw[NUM_LIMIT];

    //コントローラのL2,R2で起動されるまで待機
    while(Can[0] != 0x80){ pc.printf(" waiting \r"); }
    ThisThread::sleep_for(500);
    pc.printf("start!!\n\r");
    thread.start(shouldSwitch);

    for (i = 0; i < 8; i++){
        JETSON[i] = 0;
    }
    while(1){
        if(Can[0] == 0x80){
            if(Can[0] != 0x80){ led[0] = 0; }
            else{ led[0] = 1; }


            omniGyro();
            caterpillarMove(0);
            moveRail();
        }

        else if(Can[0] == 0x90){
            //jetsonへの通信来た時の処理を書く
            pc.printf("auto mode\n\r");

            if(Can[2] == 0x01){ jetson.putc(Can[2]); }      //送信1byte目
            else if(Can[2] == 0x02){ jetson.putc(Can[2]); }
            else{ jetson.putc(0x40); }

            if(Can[2] == 0x04){ jetson.putc(Can[2]); }      //送信2byte目
            else if(Can[2] == 0x08) { jetson.putc(Can[2]); }
            else{ jetson.putc(0x40); }
            jetson.putc(Can[3]);    //送った後の処理はわっしー担当
            //jetsonから通信が来た時の処理(自動で足動かす用)
            omniMovedByJetson();
        }
        for(i = 0;i < NUM_LIMIT; i++){
            sw[i] = limit[i];
        }
        pc.printf("id %d Can %d %d %d %d %d rderror: %d sw:%d%d%d%d\n\r", msg.id, Can[0], Can[1], Can[2], Can[3], Can[4], can.rderror(), sw[0], sw[1], sw[2], sw[3]);
        pc.printf("[[%d %d]] %d %d %d %d ", slope, yaw, sw[0], sw[1], sw[2], sw[3]);//9.24後で戻す
        pc.printf("\n\r");

        ThisThread::sleep_for(10);
    }

    return (0);
}


void caterpillarMove(int direction){    //キャタピラ動かす
    if(isCaterpillarLocked){
        caterpillar_P = 0;
        return;
    }
    pc.printf("caterpillarMove-> ");
    if(forwardTmp == 9 || JETSON[2] == 0x01){
        caterpillar_D = direction;
        if(!isOmniLocked){ caterpillar_P = MIN_CATERPILLAR_SPEED; }
        else{ caterpillar_P = MAX_CATERPILLAR_SPEED; }
    }
    else if(forwardTmp == 6 || JETSON[2] == 0x02){
        caterpillar_D = 1;
        if(!isOmniLocked){ caterpillar_P = MIN_CATERPILLAR_SPEED; }
        else{ caterpillar_P = MAX_CATERPILLAR_SPEED; }
    }
    else{
        caterpillar_P = 0;
    }
}


void shouldSwitch(void){     //キャタピラの切り替えを判断する関数
    double distance_F;
    pc.printf("thread start!!\n\r");
    while(true){
        distance_F = ultrasonic1.getDistance();  //前方の角材との距離
        pc.printf("<dist>%.2f <maxNum>%.2f", distance_F, maxNum(&omniSpeedTmp[0]));
        if(Can[2] != 0x02 && (distance_F <= SWITCH_DISTANCE) && (forwardTmp == 9 || forwardTmp == 6 || JETSON[2] == 0x01) && (maxNum(&omniSpeedTmp[0]) >= 0.35) || Can[5] == 0x02){    //ロボットの角度も判断材料
            //ここに開始点、中間点、終点の処理
            pc.printf("Caterpillar-on ");
            isCaterpillarLocked = false;
            caterpillarDown();  /*オムニ→キャタピラに切り替え*/
            stopOmni();
            isOmniLocked = true;
            while(slope <= END_ANGLE){pc.printf("1\n\r");if(Can[5] == 0x01){break;}}
            while(slope >= 3 || slope <= -3){pc.printf("2\n\r");if(Can[5] == 0x01){break;}}

        }
        else{
            pc.printf("Caterpillar-off ");
            isOmniLocked = false;
            caterpillarUp();
            isCaterpillarLocked = true;
            //後ろの超音波センサーの処理を削除
        }
        ThisThread::sleep_for(10);
    }
}

//-------ジャイロ用のオムニコード---------
void omniGyro(void){
    int i;
    int j;
    float count;
    double x,y;
    double speed;
    double angle;
    double omniPower[NUM_OMNI];
    double omniSpeed[NUM_OMNI];     //オムニpwmに書き込む値
    uint8_t forward;

    static bool flag_adjust = false;
    static float yaw_fixed = 0;
    bool lflag = false;
    bool rflag = false;

    x = (double)(Can[3]-64) / 64.0;
    y = (double)(-Can[4]+64) / 64.0;
    speed = sqrt(x*x + y*y);
    if(speed > MAX_OMNI_SPEED){
        speed = MAX_OMNI_SPEED;
    }
    angle = atan2(y, x);//ラジアンなので注意

#if 0
    if(x == 0){
            if(y >= 0){ angle = PI/2; }//ここがONだと右移動ができなくなる
            else{ angle = -PI/2; }
        }
    if( x < 0 ){ angle += PI; }
#endif


    omniPower[0] = cos(PI/4 - angle);
    omniPower[1] = cos(PI/4 + angle);
    omniPower[2] = cos(3*PI/4 + angle);
    omniPower[3] = cos(3*PI/4 - angle);

    if(Can[2] == 0x02 || Can[2] == 0x0a){
        for(i = 0; i < NUM_OMNI; i++){ omniSpeed[i] = omniPower[i] * speed; }

    }
    else{
        for(i = 0; i < NUM_OMNI; i++){ omniSpeed[i] = omniPower[i] * speed * MIN_OMNI_SPEED; }
    }

    if(isPressed(Can[2],2)){
        //左回転
        pc.printf("[roll left]\n\r");
        lflag = true;
        flag_adjust = false;
        for(i = 0; i < NUM_OMNI; i++){ Can[2] == 0x06 ? omniSpeed[i] = -MAX_ROLL_SPEED : omniSpeed[i] = -MIN_ROLL_SPEED; }
    }else{ lflag = false; }

    if(isPressed(Can[2], 4)){
        //右回転
        pc.printf("[roll right]\n\r");
        rflag = true;
        flag_adjust = false;
        for(i = 0; i < NUM_OMNI; i++){ Can[2]== 0x12 ? omniSpeed[i] = MAX_ROLL_SPEED : omniSpeed[i] = MIN_ROLL_SPEED; }
    }else{ rflag = false; }


    forward = forward & 0x00;
    for(i = 0; i < NUM_OMNI; i++){
        if(omniSpeed[i] > MAX_OMNI_SPEED){ omniSpeed[i] = MAX_OMNI_SPEED; }
        else if(omniSpeed[i] < -MAX_OMNI_SPEED){ omniSpeed[i] = -MAX_OMNI_SPEED; }

        if(omniSpeed[i] > 0){//ビットシフトで進行方向を保存
            omni_D[i] = 1;
            if(i != 3){
                forward++;
                forward = forward << 1;
            }else{
                forward++;
            }
            //pc.printf("1 ");
        }
        else if (omniSpeed[i] < 0){
            omni_D[i] = 0;
            if(i != 3){
                forward = forward << 1;
            }
            //pc.printf("0 ");//9.24後で戻す
            if(omniSpeed[i] <= -0.5){
                omniSpeed[i] += 0.07;//0.08
                pc.printf("<<-.->>");
            }else{
                omniSpeed[i] += 0.006;//0.007だと右にそれる10.3
            }
        }
    }

    if(!isOmniLocked){
        if((forward != forwardTmp) && (forward != 0)){
        //加速
            pc.printf("accel!!!! ");
            accelControl(&omniSpeed[0], 1);
        }
        else if((forward != forwardTmp) && (forward == 0)){
            //減速
            pc.printf("slow!!! ");
            accelControl(&omniSpeedTmp[0], 2);
        }
        else{
            for(i = 0; i < NUM_OMNI; i++){
                omni_P[i] = fabs(omniSpeed[i]);
            }
        }
    }
    for(i = 0; i < NUM_OMNI; i++){ omniSpeedTmp[i] = fabs(omniSpeed[i]); }
    forwardTmp = forward;

}

//配列に一気に書き込む
void accelControl(double *targetSpeed, int mode){
    int i;
    int countBreak;
    double pretime;
    double omniSpeed[NUM_OMNI];
    if(mode == 1){
        //加速
        timer_w.reset();
        timer_w.start();
        while(1){
            pretime = timer_w.read();
            for (i = 0; i < NUM_OMNI; i++) {
                omniSpeed[i] = targetSpeed[i] * (ACCEL_TIME * pretime);
                omni_P[i] = fabs(omniSpeed[i]);
                pc.printf("[%f] [%f] %f\n\r", targetSpeed[i], omniSpeed[i], ACCEL_TIME * pretime);
            }
            if(ACCEL_TIME*pretime >= 1){
                break;
            }
        }
        timer_w.stop();
    }
    else if(mode == 2){
        //減速
        countBreak = 0;
        timer_w.reset();
        timer_w.start();
        while(1){
            pretime = timer_w.read();
            for (i = 0; i < NUM_OMNI; i++) {
                if(omniSpeed[i] < 0 || fabs(targetSpeed[i]) < 0){
                    omni_P[i] = 0;
                    countBreak++;
                    break;
                }
                else if(maxNum(&omniSpeedTmp[0]) > 0.5){
                    omniSpeed[i] = 0.5 + (-ACCEL_TIME * pretime);
                    omni_P[i] = fabs(omniSpeed[i]);
                }
                else{
                    omniSpeed[i] = fabs(targetSpeed[i]) + (-ACCEL_TIME * pretime);
                    omni_P[i] = fabs(omniSpeed[i]);
                }
                pc.printf("[%f] [%f] %f\n\r", targetSpeed[i], omniSpeed[i], ACCEL_TIME * pretime);
            }
            if(ACCEL_TIME*pretime >= 1 || countBreak == 0){
                break;
            }
        }
        timer_w.stop();
    }
}

double maxNum(double *num){
    int i;
    double tmp = num[0];
    for(i = 1; i < NUM_OMNI; i++){
        if(num[i] > tmp){ tmp = num[i]; }
    }
    return tmp;
}

bool isPressed(unsigned char data, int bits){
    if((1 << bits & data) == 0){
        return false;
    }
    else{
        pc.printf("PRESS\n\r");
        return true;
    }
}
//---------------------------------------------------------




void initWheels(void){
    int i;

    initCan();                                      //CANを初期化
    Config();                                       //UART割り込みを設定

    for(i = 0; i < NUM_OMNI; i++){                  //オムニのpwm周期を設定
        omni_P[i].period_us(PERIOD_OMNI);
    }
    for(i = 0; i < NUM_UPDOWN; i++){                //パワーウィンドウのpwm周期設定
        updown_P[i].period_us(PERIOD_UPDOWN);
    }
    caterpillar_P.period_us(PERIOD_CATERPILLAR);    //キャタピラのpwm周期設定
    for (i = 0; i < NUM_LIMIT; i++){                //リミットスイッチをプルアップモードに
        limit[i].mode(PullUp);
    }
    pc.printf("set up wheels");
}


void omniAcceleration(void){
    int i;
    double pretime;
    double omniSpeed=0;

    if(CURRENT_OMNI_SPEED >= MAX_OMNI_SPEED){
        return;
    }

    timer_w.reset();
    timer_w.start();
    while(omniSpeed < MAX_OMNI_SPEED){
        pretime = timer_w.read();
        omniSpeed = 25 * pretime * pretime;
        for (i = 0; i < NUM_OMNI; i++) {
            omni_P[i] = omniSpeed;
        }
        pc.printf("time:%.2f up speed:%.2f\n\r",pretime,omniSpeed);
    }
    timer_w.stop();
    CURRENT_OMNI_SPEED = omniSpeed;
}

void omniSlowDown(void){
    int i,j;
    double pretime;
    double omniSpeed = CURRENT_OMNI_SPEED;

    if(CURRENT_OMNI_SPEED == 0){
        return;
    }
    pc.printf("omniSlowDown\n\r");

    timer_w.reset();
    timer_w.start();
    while(omniSpeed > 0){
        pretime = timer_w.read();
        omniSpeed = -25 * pretime * pretime + MAX_OMNI_SPEED;
        for (i = 0; i < NUM_OMNI; i++) {
            omni_P[i] = omniSpeed;
        }
        pc.printf("time:%.2f down speed:%.2f\n\r",pretime,omniSpeed);
    }
    timer_w.stop();
    CURRENT_OMNI_SPEED = omniSpeed;
}

inline void stopAllWheels(void){    //足回りを停止させる
    int i;

    for(i = 0; i < NUM_OMNI; i++){
        omni_P[i] = 0;
    }
    for(i = 0; i < NUM_UPDOWN; i++){
        updown_P[i] = 0;
    }
    caterpillar_P = 0;
    pc.printf("stop wheels");
}

inline void stopOmni(void){     //オムニを停止させる
    pc.printf("stop omni!\n\r");
    int i;
    for(i = 0; i < NUM_OMNI; i++){
            omni_P[i] = 0;
    }
}



void caterpillarDown(void){ //キャタピラ下げる

    if(!limit[1] && !limit[3]){ return; }
    timer_r.reset();
    timer_r.start();
    while(limit[1] || limit[3]){
        pc.printf("C Down\n\r");
        if(timer_r.read() >= CATERPILLAR_BREAK_TIME){ break; }
        updown_D[0] = 0;
        updown_D[1] = 0;
        updown_P[0] = MAX_UPDOWN_SPEED;
        updown_P[1] = MAX_UPDOWN_SPEED;
    }
    caterpiBreakTime = 0;
    timer_r.stop();
    updown_P[0] = 0;
    updown_P[1] = 0;
}


void caterpillarUp(void){   //キャタピラ上げる

    if((!limit[0] && !limit[2]) || caterpiBreakTime >= CATERPILLAR_BREAK_TIME){ return; }
    timer_r.reset();
    timer_r.start();
    while(limit[0] || limit[2]){
        pc.printf("C UP\n\r");
        if(timer_r.read() >= CATERPILLAR_BREAK_TIME){ break; }
        updown_D[0] = 1;
        updown_D[1] = 1;
        updown_P[0] = MIN_UPDOWN_SPEED;
        updown_P[1] = MIN_UPDOWN_SPEED;
    }
    caterpiBreakTime = timer_r.read();
    timer_r.stop();
    updown_P[0] = 0;
    updown_P[1] = 0;
}

void omniMovedByJetson(void){       //Jetsonからの通信で足回り動かす
    if(JETSON[0] != 0x80){return;}  //信号が来ていなければreturn
    //------
    int i;
    if(JETSON[1] == 0x01){
        if(JETSON[3] == 0x01){
            omni_D[0] = 1;
            omni_D[1] = 1;
            omni_D[2] = 1;
            omni_D[3] = 1;
            omniAcceleration();
            //setOmniPwm(0.1);//値がずっと0.1になる
        }
        else if(JETSON[3] == 0x02){
            omni_D[0] = 0;
            omni_D[1] = 0;
            omni_D[2] = 0;
            omni_D[3] = 0;
            omniAcceleration();
            //setOmniPwm(0.1);
        }
        else if(JETSON[2] == 0x01){
            omni_D[0] = 1;
            omni_D[1] = 0;
            omni_D[2] = 0;
            omni_D[3] = 1;
            omniAcceleration();
        }
        else if(JETSON[2] == 0x02){
            omni_D[0] = 0;
            omni_D[1] = 1;
            omni_D[2] = 1;
            omni_D[3] = 0;
            omniAcceleration();
        }
        else if(JETSON[2] == 0x04){
            omni_D[0] = 0;
            omni_D[1] = 0;
            omni_D[2] = 1;
            omni_D[3] = 1;
            omniAcceleration();
        }
        else if(Can[5] == 0x08){
            omni_D[0] = 1;
            omni_D[1] = 1;
            omni_D[2] = 0;
            omni_D[3] = 0;
            omniAcceleration();
        }
        else{
            omniSlowDown();
        }

    }

    else if(JETSON[1] == 0x02){//0x02はストップ
        //stop
        omniSlowDown();
    }
}


void initCan(void){
    can.reset();
    can.frequency(500000);
    can.filter(CAN_WHEEL_ID, 0xfff, CANStandard, 0);
}


//割り込み１
inline void Config(void){
    //初期設定
    jetson.attach(&JetsonData, Serial::RxIrq);
    can.attach(&canData, CAN::RxIrq);
}

//割り込み２
inline void JetsonData(void){
    //受信割り込みのときに来るよ
    int JETSON_Data = jetson.getc();
    static int bits = 0;

    if(JETSON_Data == 128){
        bits = 0;
    }

    if (JETSON_Data >= 0){
        JETSON[bits] = JETSON_Data;

        if (bits == 7){
            bits = 0;
        }
        else {
            bits++;
        }
    }
}

//割り込み3
void canData(void){
    can.read(msg);
    if(can.rderror() != 0){
        can.reset();
    }
    if(msg.id == CAN_WHEEL_ID){
        //Can = msg.data; //CAN[]にデータを代入
        for(int i = 0; i < CAN_SIZE; i++){
            Can[i] = msg.data[i];
        }
        switch(Can[7]){
            case 0:
                yaw = Can[6];
                slope = Can[1];
                break;
            case 1:
                yaw = Can[6] * -1;
                slope = Can[1];
                break;
            case 2:
                yaw = Can[1];
                slope = Can[1] * -1;
                break;
            case 3:
                yaw = Can[6] * -1;
                slope = Can[1] * -1;
                break;
        }

        led[2] = !led[2];
    }
    else{
        //Can = NULL;
        led[2] = 0;
    }
}
