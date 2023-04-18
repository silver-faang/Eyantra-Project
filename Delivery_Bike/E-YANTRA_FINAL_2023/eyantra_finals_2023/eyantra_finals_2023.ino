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
#define BUZZER        30
#define VBAT          A7
#define steerpin      45                         //44
#define delpin        44                         //45
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
bool ddflag=true;
bool rwflg=true;
bool speedflag=false;
bool turnflag=false;
bool tf=true;
long ttime=0;
float proportional=0; float integral=0; float last_proportional=0; float error=0; float derivative=0; float last_error=0; float integral1=0;
int align =2000;

float X1 = 75.0; 
float X2 = 15;   
float X3 = 0.08;  
float loop_time = 10;  

int pwm_s = 0;
byte dir;
int32_t motor_speed; 
uint32_t timer;
long currentT, previousT_1, previousT_2 = 0;  
long ft=0;
long endtime=0; 
long deltime=0;
long deltime2=0;
long deltime3=0;
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
bool bf=false;
uint8_t i2cData[14]; // Buffer for I2C data

float kp=2.8;
float ki=0;
float kd=0;

char cs='S';//current state initially start

String dls="AOOHEE"; //vector for sorted deliver locations  // AOOHEE
char ns=dls[0];;//next state initially 1st delivery location

int i=0;
int bike_sp;
int nspeed=150;
int ospeed=240;
int breakspeed=10;
int tvar=1500;
int tangle;
bool integralflag=false;
void setup() {
  steering.attach(steerpin);
  del.attach(delpin);
 TCCR2B = 0b00000010;
 TCCR2A = 0b00000011;
 
  Serial.begin(115200);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);


  pinMode(BRAKE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(BRAKE, HIGH);
  pinMode(PWM, OUTPUT);
  digitalWrite(PWM, HIGH);
  delay(1000);
  angle_setup();


Serial.println(dls);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  delay(1000);
  digitalWrite(BUZZER,HIGH);
  
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

//  if((a+b+c+d+e)<3300){flag1=true;}
//if(flag1 && (a+b+c+d+e)>4600){ flag2=true;}
//if(flag1 && flag2)
//{cp=cp+1; flag1=false; flag2=false; flagdeliver=false;}
//if(cs=='O' && dl==2){
//  if((a+b+c+d+e)<=3600 ){flag6=true;}
//if(flag6 && (a+b+c+d+e)>=3700){ flag7=true;}
//if(flag6 && flag7)
//{dl=dl+1; flag6=false; flag7=false; bhool=true; tf=true; rwflg=true; ddflag=true;}
//else{bhool=false;}
//}
//else {
if((a+b+c+d+e)<=3500 ){flag6=true;}
if(flag6 && (a+b+c+d+e)>=3700 && e>=700 && c>=700 ){ flag7=true;}
if(flag6 && flag7)
{dl=dl+1; flag6=false; flag7=false; bhool=true; tf=true; rwflg=true; ddflag=true;}
else{bhool=false;} 
Serial.print("a:");
Serial.print(a);

//Serial.print("dl suuuummm:");
//Serial.print(b+c+d+e);
Serial.print("total suuuummm:");
Serial.print(a+b+c+d+e);
//Serial.print("     cp is: ");
//Serial.print(cp); 
Serial.print("     e: ");
Serial.print(e);
Serial.print("     c: ");
Serial.println(c);
Serial.print("     dl: ");
Serial.print(dl); 
//Serial.print("     current state: ");
//Serial.print(cs); 
//Serial.print("     next state: ");
//Serial.print(ns); 
//Serial.print("     iterator: ");
//Serial.println(i); 


if(cs==ns)
{ 
 if(ddflag)
  { deltime=currentT+4000; ddflag=false;}
 if(deltime-currentT>3000){  speedflag=true; Serial.print("first delivery");flagdeliver=true;}
 else if (deltime-currentT>2000){ speedflag=true; Serial.print(" wawawwaaps"); flagdeliver=false;}
  else if (deltime-currentT>1000){ speedflag=true; flagdeliver=true; Serial.print("second delivery");  }
  else if (deltime-currentT>0){ speedflag=false; bike_sp=200; }
  else{ Serial.print("sab kuch normal"); cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}
}

if((cs=='S') && (ns=='A')) 
   {
     kp=2.8; ki=0.004;
     if( dl==5){flagdeliver=true;  cp=0; dl=0;  cs=ns;  i++; ns= dls[i];  }
      align=2000;
      bike_sp=nspeed;          
      }
                  
if((cs=='S') && (ns=='H')) 
     {  kp=2.8; ki=0.004;
       if( dl>5 && dl<7) {bike_sp= ospeed;}
       else{bike_sp = nspeed;}
       if (dl==8){ flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[++i]; }
       align=2000;
     }

 if((cs=='S') && (ns=='O'))    
  {kp=2.8; ki=0.004;
       if( dl>5 && dl<7) {bike_sp= ospeed;}
       else{bike_sp = nspeed;}
        if (dl==11){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[++i]; }
        align=2000;
                    }
    
if((cs=='S') && (ns=='E'))    
      { kp=2.8; ki=0.004;  tangle=70;
       if( dl>5 && dl<7) {bike_sp= ospeed;}
         else{bike_sp = nspeed;}
       if (dl==9&& rwflg){ turnflag=true;} else {turnflag=false;}
        
       if (dl==10){flagdeliver=true; cp=0; dl=0;  cs=ns;  ns= dls[++i]; }
      align=2000;
           }

if((cs=='A') && (ns=='H'))    
      { kp=2.8; ki=0.004;
         if( dl>0 && dl<2) {bike_sp= ospeed;}
             else{bike_sp = nspeed;}
         if (dl==3){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i]; }
         align=2000;
                  }

if((cs=='A') && (ns=='O'))    
     { 
     kp=2.8; ki=0.004; 
       if( dl>0 && dl<2) {bike_sp= ospeed;}
        else{bike_sp = nspeed;}
       if (dl==6){flagdeliver=true;   cs=ns; cp=0; dl=0; i++; ns= dls[i]; kp=2.8; ki=0; } 
       align=2000;
           }  
           

if((cs=='A') && (ns=='E'))    
      { kp=2.8; ki=0.004;  tangle=70; 
       if( dl>0 && dl<2) {bike_sp= ospeed;}
        else{bike_sp = nspeed;}
        if (dl==4&& rwflg){turnflag=true;} else {turnflag=false;}
        
       if (dl==5){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i]; }
       align=2000;
           }                        

if((cs=='H') && (ns=='A'))    
      { kp=2.8; ki=0.004; kd=0;
       if (dl==1&& rwflg){tangle=70;  turnflag=true;} 
      if(dl==2){ kp=2.8; ki=0; kd=0; align=1700;} else{ align=2000;}
      if(dl==3){ kp=2; ki=0; kd=0; align=1700;} 
      if(dl==4){ kp=3; ki=0.001; kd=2.5; } 
            if(dl==5){ kp=2.8; ki=0.01; kd=0; }
//           if (dl==1&& rwflg){tangle=70; tvar=2000; turnflag=true;} 
//           else if (dl==4 && rwflg){ kd=1; kp=3; ki=0.008; tangle=90; tvar=500; turnflag=true;} 
//          if(dl==2){ ki=0; kd=0; align=2300;} else{ align=2000;}
//            if(dl==5){  kp=2.8; ki=0.02; kd=1; } 
//            if(dl==6){  kp=2.5; ki=0.008; kd=1; } 
            if (dl==6){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}
        bike_sp=nspeed;
        
           }

if((cs=='H') && (ns=='O'))    
      { kp=2.8; ki=0.004; 
        if (dl==3){flagdeliver=true; cp=0; dl=0;  cs=ns; i++; ns= dls[i]; kp=2.8; ki=0; }
        bike_sp=nspeed;
        align=2000;
             }
           
if((cs=='H') && (ns=='E'))    
      { kp=2.8; ki=0.004;
        if (dl==1&& rwflg){tangle=70; turnflag=true;} else {turnflag=false;}
        if (dl==2){flagdeliver=true; cp=0; dl=0;  cs=ns; i++; ns= dls[i];}
         bike_sp=nspeed;
         align=2000;
           } 
           
if((cs=='O') && (ns=='A'))    
      { 
        kp=3; kd= 2.5; ki=0.04;
      //  if(dl==0) {kd=2; kp=3; ki=0.00;}
     if (dl==1&& rwflg){tangle=120; tvar= 1000; turnflag=true;} else {turnflag=false;}
  if(dl==2) {kd=1; kp=2.5; ki=0.008;}
  if(dl==3){kd=0; kp=3 ; ki=0.02;}
  //if(dl==4){kp=3; kd=0; ki=0.04;}
             //if(dl>=5){kp=2.8;kd=0;ki=0.004;} 
      if  (dl==5) {flagdeliver=true;  cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}
         bike_sp=nspeed-10;
         align=2000;
           }
            
if((cs=='O') && (ns=='H'))    
      { 
        kp=3; kd= 2.5; ki=0.04; 
  if(dl==2) {kd=1; kp=2.5; ki=0.008;}
  if(dl==3){kd=2; kp=2.5 ; ki=0.008;}
  if(dl==4){kp=2.8; kd=1; ki=0.004;}
   if(dl>=5){kp=2.8;kd=0;ki=0.004;}
        if( dl>5 && dl<7) {bike_sp = ospeed;}
        else{bike_sp = nspeed-10;}
        if (dl==8){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i]; }
           } 
           
if((cs=='O') && (ns=='E'))    
     { 
  if(dl==0 || dl==1) {kp=3; kd= 2.5; ki=0.04; }
  if(dl==2) {kd=1; kp=2.5; ki=0.008;}
  if(dl==3){kd=2;  kp=2.5  ; ki=0.008;}
  if(dl==4){kp=2.8; kd=1; ki=0.004;}
  if(dl>=5){kp=2.8;kd=0;ki=0.004;}
        if( dl>5 && dl<7) {bike_sp= ospeed;
         }
             else{bike_sp = nspeed;}
            if (dl==9&& rwflg){tangle=70;  turnflag=true;}    
        if (dl==10){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i]; }
       
           } 
if((cs=='E') && (ns=='A'))  
  { kp=2.8; ki=0; kd=0;
  if(dl==0){ kp=2.8; ki=0; kd=0; align=1700;} else{ align=2000;}
      if(dl==1){ kp=2; ki=0; kd=0; align=1700;} 
      if(dl==2){ kp=3; ki=0.008; kd=2.5; } 
            if(dl==3){ kp=2.8; ki=0.01; kd=0; }
              if (dl==4){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}

         bike_sp=nspeed-10;
         
           }  

if((cs=='E') && (ns=='H'))    
      { kp=2.8; ki=0.004; kd=0;
  if(dl==0){ kp=2.8; ki=0; kd=0; align=1700;} else{ align=2000;}
      if(dl==1){ kp=2; ki=0.00; kd=0; align=1700;} 
      if(dl==2){ kp=3; ki=0.008; kd=2.5; } 
            if(dl==3){ kp=2.8; ki=0.01; kd=0; }
            if( dl>4 && dl<6) {bike_sp= ospeed;}
       else{bike_sp = nspeed;}
        if (dl==7){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}
           }
if((cs=='E') && (ns=='O'))    
      { kp=2.8; ki=0.004; kd=0;
  if(dl==0){ kp=2.8; ki=0; kd=0; align=1700;} else{ align=2000;}
      if(dl==1){ kp=2; ki=0.00; kd=0; align=1700;} 
      if(dl==2){ kp=3; ki=0.0; kd=2.5; } 
            if(dl==3){ kp=2.8; ki=0.01; kd=0; }
            if( dl>4 && dl<6) {bike_sp= ospeed;}
       else{bike_sp = nspeed;}
        if (dl==10){flagdeliver=true; cp=0; dl=0;  cs=ns;  i++; ns= dls[i];}
           }

           
 if(turnflag){
  if(tf){ ttime=currentT+tvar; tf=false; }
 if(ttime-currentT>=0){steering.write(tangle);} 
 else{turnflag=false; rwflg=false;} }
 else{
whitelinefollow( a , b , c , d , e , align ,kp, ki , kd, dl, bhool);}

if(dl==1){flagdeliver=false;}
if(flagdeliver){deliver();}
else{nodeliver();} 

if (speedflag){ 
  analogWrite( enA, breakspeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
 } else{
if( (a>=500 || e>=500) && c<700) {
analogWrite( enA, 120);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);}
  else{
  analogWrite( enA, bike_sp);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  }
 
 }
  if(i==dls.length())
  { analogWrite( enA, 0); bf=true; endtime=currentT+1000; i++;
  }
  if(bf && endtime-currentT>=0)
  {digitalWrite(BUZZER, LOW); 
  }
  else{digitalWrite(BUZZER,HIGH);}
}
