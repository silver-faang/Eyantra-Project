void writeTo(byte device, byte address, byte value) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission(true);
}

void angle_setup() {
  Wire.begin();
  delay (100);
  writeTo(MPU6050, PWR_MGMT_1, 0);
  writeTo(MPU6050, ACCEL_CONFIG, accSens << 3); // Specifying output scaling of accelerometer
  writeTo(MPU6050, GYRO_CONFIG, gyroSens << 3); // Specifying output scaling of gyroscope
  delay (100);

  for (int i = 0; i < 1024; i++) {
    angle_calc();
    GyZ_offset_sum += GyZ;
    delay (5);
  }
  GyZ_offset = GyZ_offset_sum >> 10;
  digitalWrite(BUZZER, HIGH);
  delay(70);
  digitalWrite(BUZZER, LOW);
  delay(80);
  digitalWrite(BUZZER, HIGH);
  delay(70);
  digitalWrite(BUZZER, LOW);
  Serial.print("GyZ offset value = "); Serial.println(GyZ_offset);
}

void angle_calc() {
  
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);                  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 4, true);
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  Wire.beginTransmission(MPU6050);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 2, true);  
  GyZ = Wire.read() << 8 | Wire.read(); 

  AcX += AcX_offset;
  AcY += AcY_offset;  
  AcZ += AcZ_offset;
  GyZ -= GyZ_offset;

  robot_angle += GyZ * loop_time / 1000 / 65.536; 
  Acc_angle = atan2(AcY, -AcX) * 57.2958;               // angle from acc. values       * 57.2958 (deg/rad)
  robot_angle = robot_angle * Gyro_amount + Acc_angle * (1.0 - Gyro_amount);
  
  if (abs(robot_angle) > 20) vertical = false;
  if (abs(robot_angle) < 0.3) vertical = true;
  
  //Serial.print("Angle: "); Serial.println(robot_angle);
}





void Motor_control(int pwm) {
  if (pwm <= 0) {
    digitalWrite(DIRECTION, HIGH);
    pwm = -pwm;
  } else {
    digitalWrite(DIRECTION, LOW);
  }
//  setPWM(map(pwm, 0, 255, PWMVALUE, 0));
analogWrite(PWM,255-pwm);
}


void  whitelinefollow(float na,float nb,float nc,float nd,float ne, int align, float kp, float ki, float kd,int cp, bool bhool)
{ 
  int bikeposition = (0 * na + 1000 * nb + 2000 * nc + 3000 * nd + 4000 * ne) / (na+nb+nc+nd+ne);
 proportional= bikeposition - align;
 derivative = proportional - last_proportional;
 
integral = integral+proportional;
last_proportional = proportional;
if(bhool || (last_error>0 && error<=0 || error>0 && last_error<=0)){integral=0;}

error=proportional * kp + integral * ki + derivative * kd;
//if(cp==4) {integral=0;} || cp=pcp+1 last_error>0 && error<=0 || error>0 && last_error<=0
last_error=error;

 int  steer=map(error, -2000,2000,150,30);
 steering.write(steer); 
}

  // when delivery is called set servo angle to 175 degree
void deliver()
{ Serial.print("deliveryyy :");
del.write(175);}
// when nodelivery is called set servo angle to 100 degree
void nodeliver()
{del.write(100);
Serial.print(" nooooooottttt dlr :");
}
