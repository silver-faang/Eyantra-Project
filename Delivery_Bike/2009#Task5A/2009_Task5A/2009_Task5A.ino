


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

#define enA           26        //    // OC3A
#define in1           22          //PA0
#define in2           23           //PA1
#define steerpin      44       //O   
Servo steering;

int a; int b; int c; int d; int e; bool turn;
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
  digitalWrite(BRAKE, HIGH);
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
turn=decide();
if (turn) { leftturn(); }// decision 
else { straight();}
if (thisT - startT >= 15000) {

if (thisT - steerT >= 20) {
   digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
digitalWrite(enA, HIGH);
//fwd_motion();
  steerT=thisT;}
  else{
    digitalWrite(enA, LOW);
     digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);}
 flag=true;
}

}
