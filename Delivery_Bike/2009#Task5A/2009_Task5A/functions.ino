//– Function Name: write
//– Input: < Device name, its address and the value to be written
//– Output: < no retun
//– Logic: standard procedure, just writes the desired value
//– Example Call : write(MPU6050, PWR_MGMT_1, 0);
void write(byte device, byte address, byte value) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission(true);
}
//– Function Name: angle_setup()
//– Input: < nil
//– Output: < angle, 
//– Logic: standard procedure, just writes the desired value
//– Example Call : write(MPU6050, PWR_MGMT_1, 0);
void angle_setup() {
  Wire.begin();
  delay (100);
  write(MPU6050, PWR_MGMT_1, 0);                     // Talk to the register 6B
  write(MPU6050, ACCEL_CONFIG, accSens << 3);        //Talk to the ACCEL_CONFIG register  
  write(MPU6050, GYRO_CONFIG, gyroSens << 3);        // Talk to the GYRO_CONFIG register
  delay (100);

  for (int i = 0; i < 1024; i++) {
    angle_get();
    GyZ_offset_sum += GyZ;
    delay (5);
  }
  GyZ_offset = GyZ_offset_sum >> 10;
 
  Serial.print("GyZ offset value = "); Serial.println(GyZ_offset);
}
//– Function Name: angle_get()
//– Input: < nil
//– Output: < value of gyroscopic angle is updated in variables
//– Logic: standard procedure, just obtains angles from MPU6050
//– Example Call :  angle_get();
void angle_get() {
  
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);                   // Start with register 0x3B       
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 4, true);  // Read 4 registers total, each axis value is stored in 2 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  Wire.beginTransmission(MPU6050);
  Wire.write(0x47);              // Gyro data first register address
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 2, true);  // Read 2 registers total, each axis value is stored in 2 registers
  GyZ = Wire.read() << 8 | Wire.read(); 

  AcX += AcX_offset;
  AcY += AcY_offset;  
  AcZ += AcZ_offset;
  GyZ -= GyZ_offset;

  zangle += GyZ * timeloop / 1000 / 65.536; 
  Acc_angle = atan2(AcY, -AcX) * 57.2958;               // angle from acc. values       * 57.2958 (deg/rad)
  zangle = zangle * Gyro_amount + Acc_angle * (1.0 - Gyro_amount);
  
  if (abs(zangle) > 20) vertical = false;
  if (abs(zangle) < 0.3) vertical = true;
  
  Serial.print("Angle: "); Serial.println(zangle);
}


//– Function Name: Nidec_motor_control
//– Input: < pwm value which is calculated by PID logic of balancing bike
//– Output: < just the pwm value that should be given at motor pin
//– Logic: changes the sign of pwm value according to required rotation direction of motor
//– Example Call :   Nidec_motor_control(-150)
void Nidec_motor_control(int pwm) {
  if (pwm <= 0) {
    digitalWrite(DIRECTION, HIGH);
    pwm = -pwm;
  } else {
    digitalWrite(DIRECTION, LOW);
  }
 // setspeed(map(pwm, 0, 255, PWMVALUE, 0));
  analogWrite(PWM,255-pwm);
  
  }
//– Function Name: FWD_MOTION
//– Input: NIL
//– Output: NIL
//– Logic: DC MOTOR STARTS
//– Example Call :   fwd_motion()

void fwd_motion()
{  digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 //digitalWrite(enA, HIGH);
analogWrite(enA, 130);
}
//– Function Name: whitelinefollow
//– Input: < align
//– Output: < helps bike follow line
//– Logic: maps the servo steering motor's angle according to feedback of IR sensors (80 degree for straight)
//– Example Call :   whitelinefollow(2000)
void whitelinefollow(int  align)
{
 a=analogRead(A0);
 b=analogRead(A1);
 c=analogRead(A2);
 d=analogRead(A3);
e;//=analogRead(A4);
  Serial.print("a=  ");
  Serial.print(a);
  Serial.print("b=  ");
  Serial.print(b);
  Serial.print("c=  ");
  Serial.print(c);
  Serial.print(" d= ");
  Serial.print(d);
  Serial.print(" e= ");
  Serial.print(e);
  Serial.println();


int minm=200; 
int maxm=900;
if (a<=650){a=0;}else{a=1;}
if (b<=650){b=0;}else{b=1;}
if (c<=650){c=0;}else{c=1;}
if (d<=650){d=0;}else{d=1;}

  Serial.print(" a= ");
  Serial.print(a);
  Serial.print(" b= ");
  Serial.print(b);
  Serial.print(" c= ");
  Serial.print(c);
  Serial.print(" d= ");
  Serial.print(d);
 // Serial.print(" e= ");
 // Serial.print(e);

  if( a==0 && b==0 && c==0 && d ==0) {e=1;}else{e=0;}
   Serial.print(e);
 int bikeposition = (0 * a + 1000 * b + 2000 * c + 3000 * d + 4000 * e) / (a+b+c+d+e);

 int error= bikeposition - align;
// integral += proportional;
//last_proportional = proportional;
Serial.print("bikeposition=");
Serial.print(bikeposition);
 int  steer=map(error, -2000,2000,130,30);

 steering.write(steer);  
 Serial.print("steer= :");
Serial.print(steer);

}
//– Function Name: decide
//– Input: NIL
//– Output: helps the bike to make decision about turns
//– Logic: tracker sensor reads values and decides left_turn or right_turn
//– Example Call :   decide()
bool decide()
{
   a=analogRead(A0);
 b=analogRead(A1);
 c=analogRead(A2);
 d=analogRead(A3);
 e;//=analogRead(A4);
  Serial.print("a=  ");
  Serial.print(a);
  Serial.print("b=  ");
  Serial.print(b);
  Serial.print("c=  ");
  Serial.print(c);
  Serial.print(" d= ");
  Serial.print(d);
  Serial.print(" e= ");
  Serial.print(e);
  Serial.println();


int minm=200; 
int maxm=900;
if (a<=650){a=0;}else{a=1;}
if (b<=650){b=0;}else{b=1;}
if (c<=650){c=0;}else{c=1;}
if (d<=650){d=0;}else{d=1;}

  Serial.print(" a= ");
  Serial.print(a);
  Serial.print(" b= ");
  Serial.print(b);
  Serial.print(" c= ");
  Serial.print(c);
  Serial.print(" d= ");
  Serial.print(d);
 // Serial.print(" e= ");
 // Serial.print(e);
if(a+b+c+e+d>=3){
  return true;
} else {return false;}
 
}
//– Function Name: leftturn
//– Input: NIL
//– Output: turns the bike left
//– Logic: calls whitelinefollow(500), "500" setpoint turns the bike left
//– Example Call :  leftturn() 
void leftturn()
{
  whitelinefollow(500);
}
//– Function Name: rightturn
//– Input: NIL
//– Output: turns the bike right
//– Logic: calls whitelinefollow(3000), "3000" setpoint turns the bike right
//– Example Call :  righttturn() 
void rightturn()
{
  whitelinefollow(3000);
}
//– Function Name: straight
//– Input: NIL
//– Output: makes the bike follow middle path
//– Logic: calls whitelinefollow(2000), "2000" setpoint makes the bike follow line
//– Example Call :  righttturn()
void straight()
{
  whitelinefollow(2000);
}
