


#include <Servo.h>
#include <Wire.h>
#define MPU6050       0x68         // MPU6050 I2C address
#define ACCEL_CONFIG  0x1C          //Talk to the ACCEL_CONFIG register
#define GYRO_CONFIG   0x1B          // Talk to the GYRO_CONFIG register
#define PWR_MGMT_1    0x6B           // Talk to the register 6B
#define PWR_MGMT_2    0x6C            // Talk to the register 6c

#define BRAKE         24         //PA2
#define PWM           10        //PB5    //OC1A
#define DIRECTION     25           //PA3

#define enA           26         //    // OC3A
#define in1           22          //PA0
#define in2           23           //PA1
#define steerpin      44       //O  
#define deliverpin      45  
Servo steering;
Servo del; 
int da=0; int db=0; int dc=0; int dd=0; int de=0;  int minm=300; int maxm=950; long is;
int a=1; int b=1; int c=1; int d=1; int e=1; bool turn; int dl=0; float proportional; float integral=0; float last_proportional=0; float error=0; float derivative=0;
long atime=0;
int pa=1; int pb=1; int pc=1; int pd=1; int pe=1; int cp=0; long stime=0;
float k1 = 75.0;                                // k1 obtained from LQR stabilizer
float k2 = 5.25;                                // k2 obtained from LQR stabilizer
float k3 = 0.04;                                // k3 obtained from LQR stabilizer
float timeloop = 10;                            // timeloop for nidec motor
int startT=0;
int pwm = 0;                                    // the pwm which is given to nidec motor

int32_t motor_speed;                            // motor speed obtained from previous loop
long thisT; 
long lastT=0; 
long steerT = 0; 
int16_t AcX, AcY, AcZ, GyZ, gyroZ;               // angles obtained from  gyroscope

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
float zdot;
int steer;
float zangle;
float Acc_angle;
 bool flag=false;
bool vertical = false;  



void setup() {
  Serial.begin(115200);

 TCCR2B = 0b00000010;
 TCCR2A = 0b00000011; 
 
  pinMode(PWM, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(BRAKE, LOW);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  Serial.println("TRSensor example");
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  steering.attach(steerpin);
   del.attach(deliverpin);
  delay(1000);
  angle_setup();
  analogWrite(PWM, 0);
}

void loop() {
  thisT = millis();
  if (thisT - lastT >= timeloop) {
    angle_get();
    if (vertical) {
      digitalWrite(BRAKE, HIGH);
      gyroZ = GyZ / 131.0;                      // Conversion to degree/second
      
      zdot = alpha * gyroZ + (1 - alpha) * zdot;    // deciding the angular velocity of bike by using gyroscope
      pwm = -constrain(k1 * zangle + k2 * zdot + k3 * -motor_speed, -255, 255); // we used k matrix obtained from lqr method now applying feedback control U=-kX logic using optimal matrix 

   Nidec_motor_control(pwm);
      motor_speed += pwm;
    } else {
    Nidec_motor_control(0);
      digitalWrite(BRAKE, LOW);
      motor_speed = 0;
     
    }
lastT= thisT;
}

 a=analogRead(A0); b=analogRead(A1); c=analogRead(A2); d=analogRead(A3); e=analogRead(A4); float na=a; float nb=b; float nc=c; float nd=d; float ne=e;
//if(thisT-stime>=200){ 
na=(na-minm)/(maxm-minm);
nb=(nb-minm)/(maxm-minm);
nc=(nc-minm)/(maxm-minm);
nd=(nd-minm)/(maxm-minm);
ne=(ne-minm)/(maxm-minm);
//stime=thisT;}
whitelinefollow(na,nb,nc,nd,ne,2000); 
  
 
 if(thisT-atime>=50){
  da=a; db=b; dc=c; dd=d; de=e;
if (da>=800){da=1;}else{da=0;} if (db>=800){db=1;}else{db=0;} if (dc>=800){dc=1;}else{dc=0;} if (dd>=800){dd=1;}else{dd=0;} if (de>=800){de=1;}else{de=0;}


if((da-de)==1 && dc==1 && da-pa==1)
{dl=dl+1;}
if(da==de && (de-pe==1 || da-pa==1)) 
{ cp=cp+1;}


   Serial.print("deliveryb is: ");
  Serial.print(dl);
  Serial.print("c p  is: ");
  Serial.print(cp);
  Serial.print('\n');

if(dl==1 && cp==0){deliver();}
if((cp==1 || cp==2) && dl==1){nodeliver();}
if(dl==2 && cp==2){deliver();}
if(cp==3 && dl==2){nodeliver();}
if(dl==3 && cp==3){deliver();}
if(cp==4 && dl==3){nodeliver();}
if(cp==0 && dl==0){nodeliver();}
 atime=thisT;
 
 }
 

Serial.print("check points :");
Serial.print(cp);
Serial.print("count del :");
Serial.print(dl);


if (thisT - steerT >4) {
digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(enA, HIGH);              //fwd_motion
  steerT=thisT;}
 else{
 digitalWrite(enA, 0);
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
}

pa=da; pb=db; pc=dc; pd=dd; pe=de;

}
