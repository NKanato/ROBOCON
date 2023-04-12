/*
 *2023_Kyorobo_ver2
 *2023年九州共同ロボコン　沖縄高専美らヤドカリーズ　ソースコード
 *
 *ver3の変更点:
 *出来るだけコード整形した完成版(のはず)
 *
 *Tips:
 *ボーレートは115200
 *
 *
*/

#include "mbed.h"
#include "rtos.h"
#include "RotaryEncoder.h"
#include "servo.h"
#include "math.h"



/******定数(define)******/
#define MAX_ROLL_SPEED 0.5          //ガチャ盤の速度を制限
#define MAX_UPDOWN_SPEED 0.4        //ガチャが上下のする速度
#define MAX_OMUNI_SPEED 0.2
#define MAX_ARM1_SPEED 0.4
#define MAX_ARM2_SPEED 0.4
#define MAX_CUP_SPEED 0.5
#define M_PI 3.14159265358979323846


/******定数(const)******/
const uint32_t TIMEOUT_MS = 5000;           //watchdogタイマのタイムリミット
//const float MAX_OMUNI_SPEED = 0.4;


/******関数宣言******/
inline void Config(void);
inline void PS3Data(void);
void gatya_roll(void);          //ガチャガチャを回す関数
void gatya_updown(void);        //ガチャガチャを昇降・下降させる
void cup_updown(void);
void kick_wd(void);             //watchdogタイマをキックする
//void MoveOmni(void);          //スレッド化したので一旦コメントアウト3.2.2023
void OmniPWM(int speed);
void setup_PWM(void);
void move_arm1(void);//ユンボ動かす
void move_arm2(void);//UFOキャッチャー動かす
float calc_Rightjoyangle(void);//右joysticの角度を算出
float calc_Leftjoyangle(void);//左joysticの角度を算出
void MoveServo(void);//サーボ動かす



/******PIN宣言******/
RawSerial pc(USBTX,USBRX);              //シリアル通信
RawSerial cont(PC_10, PC_11, 2400);      //SBXBT用。ボーレートを2400に設定
RotaryEncoder R1(PA_0, PA_1);           //ガチャガチャのエンコーダ
DigitalOut motor(PC_0);
PwmOut motor_p(PA_10);//PC_7でチャンネルがかぶってたので変更


//足回り(オムニ)digitalout
DigitalOut D_omuni[4] = {
    DigitalOut(PC_5),
    DigitalOut(PB_12),
    DigitalOut(PB_9),
    DigitalOut(PB_8)
    };

//足回り(オムニ)PwmOut
PwmOut P_omuni[4] = {
    PwmOut(PB_4),//捨てピン//PB_10
    PwmOut(PB_5),
    PwmOut(PC_8),
    PwmOut(PC_9)
    };
PwmOut P_omuni0(PC_6);


DigitalOut motorD_updown(PC_2);     //ガチャ昇降用モーター
PwmOut motorP_updown(PB_7);         //上に同じ

DigitalOut cupD_updown(PC_4);       //カップ昇降用モーター
PwmOut cupP_updown(PA_11);

DigitalIn limit[2] = {
    DigitalIn(PC_13),               //下：リミットスイッチ(PC_13はどうせユーザースイッチだから)
    DigitalIn(PA_15)                //上：PC_14は使えないピンだった;_;
};

DigitalOut motorD_arm1(PA_7);               //ユンボー
PwmOut motorP_arm1(PB_6);
DigitalOut motorD_arm2(PA_9);                   //UFOキャッチャーのモーター
PwmOut motorP_arm2(PA_8);
servo servo_arm2(PB_10);                    //PB_4から変更//UFOキャッチャーのサーボ

DigitalOut led(PA_4);


/******グローバル変数******/
int PS3[7];         //PS3コントローラーの電文格納変数
int countL1;    //L1スイッチのカウンタ

Timer timer;
Thread thread;


//関数だとうまくいかなかったのでスレッドにした
void MoveOmni(){
    pc.printf("omnistart!\n\r");
    P_omuni0.period_us(1275);           //オムニ
    P_omuni[1].period_us(1275);
    P_omuni[2].period_us(1275);
    P_omuni[3].period_us(1275);
    //float speed;
    //float *p;
    //p = &speed;
    float angle;
    while(1){
        angle = calc_Rightjoyangle();


        if(PS3[1] == 0x10){
            if(angle > -22.5 && angle <= 22.5){
                D_omuni[0] = 0;//右に進む
                D_omuni[1] = 0;
                D_omuni[2] = 0;
                D_omuni[3] = 0;
                pc.printf(" Rolling_RIGHT ");
                OmniPWM(1);
            }
            else if(angle > 157.5  && angle <= -157.5 || angle == 180){
                D_omuni[0] = 1;//左に進む
                D_omuni[1] = 1;
                D_omuni[2] = 1;
                D_omuni[3] = 1;
                pc.printf(" Rolling_LEFT ");
                OmniPWM(1);
            }
            else{
                //pc.printf(" ELSE ");
                OmniPWM(0);
            }
        }


        else{
            if(angle > -22.5 && angle <= 22.5 || angle == 0){
                
                D_omuni[1] = 0;
                D_omuni[2] = 0;
                D_omuni[3] = 1;
                D_omuni[0] = 1;//右に進む
                pc.printf(" RIGHT \n\r");
                OmniPWM(1);
            }
            
            else if(angle > 67.5 && angle <= 112.5){
                
                D_omuni[1] = 0;
                D_omuni[2] = 1;
                D_omuni[3] = 1;
                D_omuni[0] = 0;//前に進む
                pc.printf(" GO \n\r");
                OmniPWM(1);
            }
            
            else if(angle > 157.5  && angle <= -157.5 || angle == 180){
                
                D_omuni[1] = 1;
                D_omuni[2] = 1;
                D_omuni[3] = 0;
                D_omuni[0] = 0;//左に進む
                pc.printf(" LEFT \n\r");
                OmniPWM(1);
            }
            
            else if(angle > -112.5 && angle <= -67.5){
                
                D_omuni[1] = 1;
                D_omuni[2] = 0;
                D_omuni[3] = 0;
                D_omuni[0] = 1;//後ろに進む
                pc.printf(" BACK \n\r");
                OmniPWM(1);
            }
            else{
            //pc.printf(" ELSE ");
                OmniPWM(0);
            }
        }
        //pc.printf("\r");
    }
}


void cup_updown(void){
    //pc.printf(" cup_updown\n\r ");
    /*int L[2];
    L[0] = limit[0];
    L[1] = limit[1];*/
    //pc.printf(" %d %d ",L[0],L[1]);
    //pc.printf(" cup_updown ");
        if (PS3[1] == 0x01){
            while(1){
                pc.printf(" cup_updown\n\r");
                while(limit[1] == 1){
                    pc.printf("cup up!!\n\r");
                    cupD_updown = 0;
                    cupP_updown = MAX_CUP_SPEED;
                }
            //if(limit[0] == 1 && limit[1] == 0)
                while(limit[0] == 1){
                    pc.printf("cup down!!\n\r");
                    cupD_updown = 1;
                    cupP_updown = MAX_CUP_SPEED;
                }
            
            cupP_updown = 0;
            pc.printf(" cup_def!! ");
            break;
            }

            
            
        kick_wd();
        }
    
}




int main()
{
    int i,j;                            //ループ用変数
    int Limit[2];
    pc.printf("START!!\n\r");
    Watchdog &watchdog = Watchdog::get_instance();        //Watchdog APIのwatchdogインスタンス生成
    watchdog.start(TIMEOUT_MS);         //watchdog開始
    setup_PWM();                        //モーターのpwm設定
    servo_arm2.Set(0, 180, 0.0005, 0.0025, 0.02);
    pc.baud(115200);
    for(i = 0; i < 2; i++){
        limit[i].mode(PullUp);
    }
    led = 0;
    timer.start();                      //タイマー開始
    Config();                           //SBXBT割り込み
    MoveServo();
    
    
    //Init_Omni();
    while(1){
        if(PS3[1] == 0x14){
            break;
        }
        else{
            pc.printf(" waiting\n\r");
        }
    }

    thread.start(MoveOmni);                     //オムニ用スレッド開始
    //thread.start(cup_updown);
    led = 1;

    while (1) {

        /*for (i = 0; i < 7; i++){
            pc.printf("%x, ", PS3[i]);
        }*/
        motor_p = 0;

        if (PS3[0] == 0x80){
            Limit[0] = limit[0];
            Limit[1] = limit[1];
            gatya_roll();           //ガチャ盤を回転させる関数
            gatya_updown();         //ガチャ昇降させる関数
            cup_updown();           //コップを自動で上下させる
            //MoveOmni();             //オムニ動かす
            move_arm1();            //ユンボー動かす
            move_arm2();            //ＵＦＯキャッチャー動かす
            MoveServo();            //サーボ動かす
            pc.printf(" %d %d ",Limit[0], Limit[1]);
        }
        



        //pc.printf("%d :%.4f\n\r", R1.Get_Count() ,R1.Get_Count()*0.17578125);
        pc.printf("\n\r");          //watchdog timerをキックする
        kick_wd();
        ThisThread::sleep_for(10);
        
    }
}


//arm1(ユンボー)動かす
void move_arm1(void){
    float angle;
    angle = calc_Leftjoyangle();
    if(PS3[1] == 0x04){
        if(angle > 67.5 && angle <= 112.5){
            pc.printf(" Rolling Arm1");
            motorD_arm1 = 0;
            motorP_arm1 = MAX_ARM1_SPEED;
        }
        else {
            motorP_arm1 = 0;
        }
    }
    else {
        motorP_arm1 = 0;
    }
}


//arm2(UFOキャッチャー)動かす
void move_arm2(void){
    float angle;
    angle = calc_Leftjoyangle();
    //MoveServo();
    if(angle > 67.5 && angle <= 112.5 && PS3[1] != 0x04){
        pc.printf(" Rolling Arm2 ");
        motorD_arm2 = 0;
        motorP_arm2 = MAX_ARM2_SPEED;
    }
    else {
        motorP_arm2 = 0;
    }
}

void MoveServo(void){
    //pc.printf(" moveservo ");
    int i = 0;
    
        while(PS3[1] != 0x02){
            ThisThread::sleep_for(5);
            return;
            //i++;
            //if(i < 5)return;
        }
        countL1++;
        if(countL1 == 1){
            pc.printf(" SeClose ");
            servo_arm2.Position(170);
        }
        else if(countL1 == 2){
            pc.printf(" SeOpen ");
            servo_arm2.Position(90);
            countL1 = 0;
        }
        i = 0;
        while(PS3[1] == 0x02)ThisThread::sleep_for(5);//pc.printf(" 0x02 ");

}



void OmniPWM(int speed){
    //pc.printf(" OmniPWM ");
    P_omuni0 = speed * MAX_OMUNI_SPEED;
    //pc.printf(" 0->");
    P_omuni[1] = speed * MAX_OMUNI_SPEED;
    //pc.printf(" 1->");
    P_omuni[2] = speed * MAX_OMUNI_SPEED;
    //pc.printf(" 2->");
    P_omuni[3] = speed * MAX_OMUNI_SPEED;
    //pc.printf(" 3->");
}


//右ジョイスティックの角度を算出する
float calc_Rightjoyangle(void)
{
	int origin = 64;
	float Vx;
	float Vy;
	float V;
	float joyAngle;
	
	Vx = PS3[5] - origin;
	Vy = PS3[6] - origin;
	
	V = sqrt((pow(Vx,2) + pow(Vy,2)));
    
    /*if(V>64){
        *tmp = V - 26;
    }
    else {
        *tmp = V;
    }*/
	joyAngle = acos(Vx/V)/(M_PI/180);

	//printf("ベクトルの大きさ=%f\n角度=%f°",V,joyAngle);
    if(PS3[6] > 64){
        joyAngle = joyAngle * (-1);
    }
    //pc.printf(" >>%.2f ",joyAngle);
	return (joyAngle);
}


//左スティックのジョイスティックの角度を算出
float calc_Leftjoyangle(void)
{
	int origin = 64;
	float Vx;
	float Vy;
	float V;
	float joyAngle;
	
	Vx = PS3[3] - origin;
	Vy = PS3[4] - origin;
	
	V = sqrt((pow(Vx,2) + pow(Vy,2)));
    
    
	joyAngle = acos(Vx/V)/(M_PI/180);

	//printf("ベクトルの大きさ=%f\n角度=%f°",V,joyAngle);
    if(PS3[6] > 64){
        joyAngle = joyAngle * (-1);
    }
    
    //pc.printf(" >>%.2f ",joyAngle);
	return (joyAngle);
}


//コップを上下させる
/*void cup_updown(void){
    int L[2];
    L[0] = limit[0];
    L[1] = limit[1];
    pc.printf(" %d %d ",L[0],L[1]);
    //pc.printf(" cup_updown ");
    if (PS3[1] == 0x01){
        if(limit[0] == 1 && limit[1] == 1){
                pc.printf("cup up!!\n\r");
                cupD_updown = 0;
                cupP_updown = MAX_ROLL_SPEED;
            }
            if(limit[0] == 1 && limit[1] == 0){
                pc.printf("cup down!!\n\r");
                cupD_updown = 1;
                cupP_updown = MAX_ROLL_SPEED;
            }
            else if (limit[0] == 0 && limit[1] == 1){
                pc.printf("Back2Def!!\n\r");
                cupD_updown = 0;
                cupP_updown = MAX_ROLL_SPEED;
                ThisThread::sleep_for(100);
                cupP_updown = 0;

            }
        kick_wd();
    }
}*/


//ガチャ上下させる
void gatya_updown(void){

    //pc.printf(" gatya_updown ");
    switch (PS3[2]){
        case 0x10:                          //↑ボタンが押されたら昇降する
            pc.printf("go up!!");
            motorD_updown = 1;
            motorP_updown = MAX_UPDOWN_SPEED;
            break;

        case 0x20:                          //↓ボタンが押されたら下降する
            pc.printf("go down!!");
            motorD_updown = 0;
            motorP_updown = MAX_UPDOWN_SPEED;
            break;

        default :                           //デフォルトではパワー0
            //pc.printf("up down default");
            motorP_updown = 0;
    }
}


//ガチャガチャの盤を回転させる関数
void gatya_roll(void){
    //pc.printf(" gatya_roll ");
    double Target_angle = 120.0;        //目標の角度
    double Current_angle = 0.0;         //現在の角度
    double dt;                          //時間の変化量
    double pretime = 0;
    double P, I, D=0, preP=0;

    //PIDゲイン
    const double kp = 0.09;         //constはdefineの変数版。変えられたくない定数に使う
    const double ki = 0.0;
    const double kd = 0;

    motor_p.period_us(1275);            //ガチャ回転の周波数設定

    if (PS3[2] == 0x40){
        while(1){
            Current_angle = R1.Get_Count() * 0.17578125;
        //pc.printf("%f\n\r",Current_angle);        //ここを実行すると、円盤がずっと回ってしまう。原因は不明。
        
            dt = (timer.read_us()-pretime)/1000000; //時間の変化量
            pretime = timer.read_us();


            //ここからPIDの計算
            P = Target_angle - Current_angle;       //距離の変化量
            if(P < 0){
                P *= -1;            //負の値にならないようにする
            }
            I += P*dt;              //積分
            D = (P - preP)/dt;      //微分
            preP = P;


            if(Target_angle < Current_angle){
                motor = 0;
                motor_p = P * kp + I * ki + D * kd;
                if(motor_p > MAX_ROLL_SPEED){           //速度制限
                    motor_p = MAX_ROLL_SPEED;
                }
            }

            else if(Target_angle > Current_angle){
                motor = 1;
                motor_p = P * kp  + I * ki + D * kd;
                if(motor_p > MAX_ROLL_SPEED){           //速度制限
                    motor_p = MAX_ROLL_SPEED;
                }
            }

            else{
                motor_p = 0;
            }
            if(Current_angle >= Target_angle){
                break;
            }
        }
    }
    else {
        motor_p = 0;
    }
    
    R1.Reset();
}


//各モーターのpwm周期を設定
void setup_PWM(void){
    motor_p = 0;
    //motor_p.period_us(1275);            //ガチャ回転の周波数設定
    motorP_updown.period_us(1275);      //ガチャ昇降の周波数設定
    cupP_updown.period_us(1275);        //カップ
    motorP_arm1.period_us(1275);        //ユンボー
    motorP_arm2.period_us(1275);        //UFO
}


//watchdog timerをキックするよ
void kick_wd(void){
    pc.printf(" KICKED ");
    Watchdog::get_instance().kick();
    //pc.printf("kicked!!\n\r");
}


//割り込み１
inline void Config(void){
    //初期設定    
    cont.attach(&PS3Data, Serial::RxIrq);
}


//割り込み２
inline void PS3Data(void)
{
    //受信割り込みのときに来るよ
    int SBDBT_Data = cont.getc();
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

/*Coded by Kanato Nino@NITOC*/
//kuso code gomen