#include <Wire.h>
#include <Servo.h>
#define MPU6050       0x68         // Device address
#define ACCEL_CONFIG  0x1C         // Accelerometer configuration address
#define GYRO_CONFIG   0x1B         // Gyro configuration address
#define PWR_MGMT_1    0x6B
#define PWR_MGMT_2    0x6C
int maxm=900; int minm=300;
#define BRAKE         24 
#define PWM           10
#define DIRECTION     25
#define BUZZER        12
#define VBAT          A7
#define steerpin      44
#define delpin        45
#define enA           11
#define in1           23
#define in2           22
Servo steering;
Servo del;
float a=0; float b=0; float c=0; float d=0; float e=0; float na=0; float nb=0; float nc=0; float nd=0; float ne=0; int ta=0; int tb=0; int tc=0; int td=0; int te=0;
int cp=0;
int dl=0;
bool flag1=false;
bool flag2=false;
bool flag3=false;
bool flag4=false;
bool flag5=false;
bool flag6=false;
bool flag7=false; 
bool bhool=false;
bool flagdeliver=false;
float proportional=0; float integral=0; float last_proportional=0; float error=0; float derivative=0; float last_error=0;
int align =2000;

float X1 = 115.0; 
float X2 = 15.25;   
float X3 = 0.04;  
float loop_time = 10;  

int pwm_s = 0;
byte dir;
int32_t motor_speed; 
uint32_t timer;
long currentT, previousT_1, previousT_2 = 0;  long ft=0;
int16_t AcX, AcY, AcZ, GyZ, gyroZ;

//Sensor output scaling
#define accSens 0             // 0 = 2g, 1 = 4g, 2 = 8g, 3 = 16g
#define gyroSens 1            // 0 = 250rad/s, 1 = 500rad/s, 2 1000rad/s, 3 = 2000rad/s
#define Gyro_amount 0.996     // percent of gyro in complementary filter

//IMU offset values
int16_t  AcX_offset = -750;
int16_t  AcY_offset = 360;
int16_t  AcZ_offset = 0;
int16_t  GyZ_offset = 0;
int32_t  GyZ_offset_sum = 0;

float alpha = 0.40; 
float gyroZfilt;

float robot_angle;
float Acc_angle;

bool vertical = false;  

uint8_t i2cData[14]; // Buffer for I2C data



char cs='S';//current state initially start
char ns;//next state initially 1st delivery location
String dls="";   //vector for sorted deliver locations
int i=0;
int bike_sp;

void setup() {
  steering.attach(steerpin);
  del.attach(delpin);
 TCCR2B = 0b00000010;
 TCCR2A = 0b00000011;
 
  Serial.begin(115200);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(BRAKE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(BRAKE, HIGH);
  pinMode(PWM, OUTPUT);
  digitalWrite(PWM, HIGH);
  delay(1000);
  angle_setup();

  //Input variables declration
  
    int n=1;//total deliveries
    int m=4;//no of delivery locations
    int f[m];//frequency array for number of items in each delivery location
    int fi[3];//frequency of total number item of each category highest priority element Medicine correspond to 0 and Food to 1 and Grocery to 2 a/c to priority
    
    char dlname[m];//vector for name of delivery location
    int v[m][2];//vector for all delivery locations items storage (with items stored in int 


   
                              
                              // such that highest priority item value=0, subsequently 1,2) 
    //taking input frm user
    f[0]=0;
    f[1]=1;
    f[2]=0;
    f[3]=0;
    fi[0]=2;
    fi[1]=2;
    fi[2]=2;
    dlname[0]='A';
    dlname[1]='H';
    dlname[2]='O';
    dlname[3]='E';
    v[0][0]=0;
    v[0][1]=1;
    v[1][0]=0;
    v[1][1]=2;//denoting blank space
    v[2][0]=1;
    v[2][1]=10;
    v[3][0]=2;
    v[3][1]=10;
    //input ends


   
    
    int c [m];
    for(int i=0;i<m;i++) c[i]=0;
    
    while(i<3){
        int j=0;
        int cnt=0;
        while(j<m){
            if(v[j][c[j]]==i){
                dls+=dlname[j];
                //cout<<dlname[j]<<" ";
                c[j]++;
                cnt++;
            }
            j++;
        }
        if(fi[i]==cnt)
         i++;
         
    }
   Serial.print("D:");
   Serial.println(dls);
     
   //iterator for delivery location
   ns=dls[0];//next state initially 1st delivery location  
}

void loop() {
  currentT = millis();
  if (currentT - previousT_1 >= loop_time) {
     
    angle_calc();
    if (vertical) {
      digitalWrite(BRAKE, HIGH);
      gyroZ = GyZ / 131.0; // Convert to deg/s
      
      gyroZfilt = alpha * gyroZ + (1 - alpha) * gyroZfilt;
      pwm_s = -constrain(X1 * robot_angle + X2 * gyroZfilt + X3 * -motor_speed, -255, 255); 

      Motor_control(pwm_s);
      motor_speed += pwm_s;
    } else {
      Motor_control(0);
      digitalWrite(BRAKE, LOW);
      motor_speed = 0;
    }
    previousT_1 = currentT;
  }
  a=analogRead(A0); b=analogRead(A1); c=analogRead(A2); d=analogRead(A3); e=analogRead(A4);  na=a;  nb=b;  nc=c;  nd=d;  ne=e;
ta=a; tb=b; tc=c; td=d; te=e;
if(ta>=850){ta=1;} else{ta=0;} if(tb>=850){tb=1;} else{tb=0;} if(tc>=850){tc=1;} else{tc=0;} if(td>=850){td=1;} else{td=0;} if(te>=850){te=1;} else{te=0;}
//na=(na-minm)/(maxm-minm);
//nb=(nb-minm)/(maxm-minm);
//nc=(nc-minm)/(maxm-minm);
//nd=(nd-minm)/(maxm-minm);
//ne=(ne-minm)/(maxm-minm);

  if((a+b+c+d+e)<3300){flag1=true;}
if(flag1 && (a+b+c+d+e)>4750){ flag2=true;}
if(flag1 && flag2)
{cp=cp+1; flag1=false; flag2=false; flagdeliver=false;}

if((a+b+c+d+e)<3300 ){flag6=true;}
if(flag6 && (a+b+c+d+e)>3800 && e>900 && c>900){ flag7=true;}
if(flag6 && flag7)
{dl=dl+1; flag6=false; flag7=false; bhool=true;}
else{bhool=false;}


Serial.print("dl suuuummm:");
Serial.print(b+c+d+e);
Serial.print("total suuuummm:");
Serial.print(a+b+c+d+e);
Serial.print("     cp is: ");
Serial.print(cp); 
Serial.print("     e: ");
Serial.print(e);
Serial.print("     dl: ");
Serial.println(dl); 
// if ( cp==1 && a<500){flag3=true;  Serial.print(" 111111 ");}
// if (cp==1 && flag3 && a>950){flag4=true;}
// if (cp==1 && flag3 && flag4 && a<500){flag5=true; Serial.print(" ho gayaaaaaa "); }
// 
//if(  (dl==14) && (flag5== false)){align=1500;Serial.print(" yesssssssssss ");   }
//else{align=1900;}
// 
//if(dl==10 && (ta+tb+tc+td+te>=3)){align=1000;} else{align=2000;}

if((cs=='S') && (ns=='A')) 
   {
     if( dl==5){flagdeliver=true;  cp=0; dl=0;  cs=ns;  ns= dls[i++];  }
      
      bike_sp=160;          
      }
                  
if((cs=='S') && (ns=='H')) 
     {
       if (dl==10){ flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
       if( dl >=6) {bike_sp= 200;}
       else{bike_sp = 170;}
     }

 if((cs=='S') && (ns=='O'))    
      {  if (dl==13){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
       if( dl>5 && dl<10) {bike_sp= 210;}
       else{bike_sp = 170;}
         
           }
    
if((cs=='S') && (ns=='E'))    
      { if (dl==12){align= 1700;} 
         else{align=2000;}
       if (dl==13){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
       if( dl>5 && dl<10) {bike_sp= 210;}
       else{bike_sp = 170;}
           }

if((cs=='A') && (ns=='H'))    
      { 
        if (dl==5){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
        if( dl>0 && dl<5) {bike_sp= 210;}
        else{bike_sp = 170;}
           }

if((cs=='A') && (ns=='O'))    
     { 
        if (dl==8){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
        bike_sp = 170;
         if( dl>0 && dl<5) {bike_sp= 210;}
        else{bike_sp = 170;}
           }  

if((cs=='A') && (ns=='E'))    
      { 
        if (dl==7){align= 1700;}
        else{ align= 2000;}
        if (dl==8){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
           }                        

if((cs=='H') && (ns=='A'))    
      { 
        if (dl>=2 )  {align= 1700; }
       else{align=2000;  }
        if (dl==7){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++];}
        bike_sp=140;
           }

if((cs=='H') && (ns=='O'))    
      { 
        if (dl==3){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++];}
        bike_sp=140;
           }
           
if((cs=='H') && (ns=='E'))    
      { 
        if (dl==2){align= 1700;}
        if (dl==3){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++];}
           } 
           
if((cs=='O') && (ns=='A'))    
      { 
        if (dl==1) {align=2300;}
        else if (dl>=2 )  {align= 1600; }
             else{align=2000;  }
      if  (dl==4) {flagdeliver=true;  cp=0; dl=0;  cs=ns;  ns= dls[i++];}
         bike_sp=130;
           }
            
if((cs=='O') && (ns=='H'))    
      { 
        if (dl==1) {align=2300;}
        else if ((dl==2) || (dl==3 ))  {align= 1600; }
             else{align=2000;  }
        if( dl>4 && dl<8) {bike_sp= 240;}
        else{bike_sp = 140;}
        if (dl==9){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
           } 
           
if((cs=='O') && (ns=='E'))    
     { 
        if (dl==1) {align=2300;}
        else if ((dl==2) || (dl==3 )||(dl==11))  {align= 1600; }
             else{align=2000;  }
        if( dl>4 && dl<8) {bike_sp= 240;}
        else{bike_sp = 140;}
        if (dl==12){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++]; }
       
           } 
if((cs=='E') && (ns=='A'))    
      { 
        if ((dl==0)||(dl==1)|| (dl==2)||(dl==3)){align= 1700;}
          else{align=2000;}
       if (dl==4){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++];}
      
       bike_sp=150;
           }  

if((cs=='E') && (ns=='H'))    
      { 
       if ((dl==0)||(dl==1)|| (dl==2)||(dl==3)){align= 1700;}
          else{align=2000;}
           if( dl>4 && dl<8) {bike_sp= 240;}
        else{bike_sp = 150;}
        if (dl==9){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[i++];}
           }
if((cs=='E') && (ns=='O'))    
      { 
        if ( (dl==0)||(dl==1)|| (dl==2)||(dl==3) ){align= 1700;}
        else{align=2000;}
         if( dl>4 && dl<8) {bike_sp= 240;}
        else{bike_sp = 140;}
        if (dl==13){ flagdeliver=true;cp=0; dl=0;  cs=ns;  ns= dls[i++];}
       
           } 
 
whitelinefollow( a , b , c , d , e , align ,3.5, 0.000 , 0, cp, bhool);

if(flagdeliver){deliver();}
else{nodeliver();} 
if( (a>=600 || e>=600) && c<600) {
analogWrite( enA, 110);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);}
  else{
  analogWrite( enA, bike_sp);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  }
}
