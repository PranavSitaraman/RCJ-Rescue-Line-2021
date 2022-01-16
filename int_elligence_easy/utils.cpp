#ifndef UTILS_CPP
#define UTILS_CPP
#include "global.h"
#include "utils.h"
void ClawRaise()
{
  ServoClaw.write(90);
  delay(1000);
}
void ClawLower()
{
  for (int x = 90; x >= 30;) {
    ServoClaw.write(x);
    x = x - 5;
    delay(16.6666666667 * 5);
  }
}
void ClawRelease()
{

  for (int x = 0; x <= 720; x++) {
    pixy.setServos(x, 90);
    delay(1);
  }
}
void ClawClamp()
{
  for (int x = 720; x >= 0; x--) {
    pixy.setServos(x, 90);
    delay(1);
  }
}
void lcount(void)
{
  lEnc += 1;
}
void rcount(void)
{
  rEnc += 1;
}
/*
   Selects a port on the mux
*/
int tcaselect(uint8_t i)
{
  if (i > 7)
    return -4;
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  return (Wire.endTransmission());
}
/*
   Initalizes the robot
   Returns if it was a success
*/
bool initRobot()
{
  
  pinMode(lMotorPwm, OUTPUT);
  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lInterrupt, INPUT_PULLUP);
  pinMode(lChannelB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lInterrupt), lcount, FALLING);
  pinMode(rMotorPwm, OUTPUT);
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rInterrupt, INPUT_PULLUP);
  pinMode(rChannelB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rInterrupt), rcount, FALLING);

  buzzer.setpin(BUZZER_PIN);
  //buzzer.tone(262, 250);
  //delay(250);
  // Select right color sensor
  tcaselect(RIGHT_COLOR);
  // Inits right color sensor
  if (!color.begin())
  {
    while (1) {
      buzzer.tone(262, 250);
      delay(250);
      Serial.println("Color RIGHT Failure!");
    }
    return false;
  }
  // Select left color sensor
  tcaselect(LEFT_COLOR);
  // Inits left color sensor
  if (!color.begin())
  {
    while (1) {
      buzzer.tone(262, 250);
      delay(250);
      Serial.println("Color LEFT Failure!");
    }
    return false;
  }
  // Selects TOF

  tcaselect(TOF);
  // Inits TOF
  tof.setTimeout(500);
  if (!tof.init())
  {
    Serial.println("TOF");
    buzzer.tone(262, 250);
    delay(250);
    return false;
  }
  tof.startContinuous();
  
  // Init pixy
  pixy.init();
  pixy.changeProg("video");
  // Attach the servo to the SERVO_PIN
  ServoClaw.attach(SERVO_PIN);
  // Move servo claw to rest position
  ServoClaw.write(95);
  delay(1000);
  // Move pixy to rest position
  //pixy.setServos(90, 500);
  //delay(1000);
  gyro.begin();
  return true;
}
/*
   Sets the left and right speeds (the speeds are from 0-1)
*/
void setMotorSpeeds(double left, double right)
{ 
  // Make sure you the speeds arent higher then 1
  if (fabs(left) >= 1)
  {
    left = copysign(1, left);
  }
  if (fabs(right) >= 1)
  {
    right = copysign(1, right);
  }
  // Checks if the left speed that we are setting is less then deadzone
  if (fabs(left) < MOTOR_DEADZONE)
  {
    // Checks if the left speed is much less then the deadzone
    if (fabs(left) < MOTOR_DEADZONE / 5)
    {
      // Since it is, we set the speed to 0
      left = 0;
    } else {
      // If it isnt, we set it to +/- the deadzone
      left = copysign(MOTOR_DEADZONE, left);
    }
  }
  // Checks if the right speed that we are setting is less then deadzone
  if (fabs(right) < MOTOR_DEADZONE)
  {
    // Checks if the right speed is much less then the deadzone
    if (fabs(right) < MOTOR_DEADZONE / 5)
    {
      // Since it is, we set the speed to 0
      right = 0;
    } else {
      // If it isnt, we set it to +/- the deadzone
      right = copysign(MOTOR_DEADZONE, right);
    }
  }
  lTarPwm = left;
  rTarPwm = right;
}
void setRawMotorSpeeds(double left, double right) {
  lCurPwm = left;
  if (left < 0)
  {
    analogWrite(lMotorPwm, -left * 255);
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
  }
  else
  {
    analogWrite(lMotorPwm, left * 255);
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
  }
  rCurPwm = right;
  if(right < 0)
  {
    analogWrite(rMotorPwm, -right * 255);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
  }else{
    analogWrite(rMotorPwm, right * 255);
    digitalWrite(rMotor1, HIGH);
    digitalWrite(rMotor2, LOW);
  }
}

/*
   This simply loops the motors
*/
void loopMotors()
{
  // Loop the motors so speeds and encoders update
  double lChange = lTarPwm - lCurPwm;
  double rChange = rTarPwm - rCurPwm;

  long curLoopMillis = millis();
  double maxChange = MAX_ACC * ((curLoopMillis - prevLoopMillis)/1000.0);
  
  setRawMotorSpeeds(lCurPwm + constrain(lChange, -maxChange, maxChange), rCurPwm + constrain(rChange, -maxChange, maxChange));
  prevLoopMillis = curLoopMillis;
}
/*
   Stops the motors using raw motor speeds because setTarPWM has a delay
*/
void stopMotors()
{
  // This fully stops the motor because set Tar PWM sets the target pwm,
  // this directly sets the motor pwm without PID being added with encoders
  setRawMotorSpeeds(0, 0);
}
/*
   A function that makes you move x encoders on one wheel and y encoders on the other with the motors going at speed speed
*/
void moveTo(int ltarget, int rtarget, double speed)
{
  // Reset Encoders
  lEnc = 0;
  rEnc = 0; 
  loopMotors();

  // Set motor speeds (copysigns are so they go in the right direction)
  setMotorSpeeds(copysign(speed, ltarget), copysign(speed, rtarget));

  // Update motors until we reach target pos
  Serial.print(ltarget);
  Serial.print("\t");
  Serial.println(rtarget);
  while (abs(lEnc) < abs(ltarget) || abs(rEnc) < abs(rtarget))
  {
    loopMotors();
    setMotorSpeeds(copysign(speed, ltarget), copysign(speed, rtarget));
  }
  stopMotors();
}
/*
   Simple Wrapper to moveTo with ENC_PER_CM calculations, so you can go forward in centimeters
*/
void forward(double dist, double speed)
{
  moveTo(dist * (ENC_PER_CM), dist * (ENC_PER_CM), speed);
}
/*
   Simple Wrapper to moveTo with ENC_PER_DEG calculations, so you can go rotate in degrees
*/
void rotate(double angle, double speed)
{
  moveTo(angle * ENC_PER_DEG, -angle * ENC_PER_DEG, speed);
}
/*
   Gathers all of the sensor data into variables
*/
void loopSensors()
{
  //Serial.println("Sensor Enter");
  // This variable is a throwaway variable, we dont use color temp anywhere
  uint16_t c;
  // Select right color sensor
  tcaselect(RIGHT_COLOR);

  //Serial.println("Sensor Enter");
  // Get data and put it into the rightColorData struct
  color.getRawData(&rightColorData.r, &rightColorData.g, &rightColorData.b, &c);

  //Serial.println("Sensor Enter");
  //Serial.println(rightColorData.g);
  // Calculate the color that it was on (BLACK, WHITE OR GREEN)
  if (rightColorData.g < BVALUE * 1.5)
  {
    rightColorData.color = BLACK;
  }
  else if ((double) rightColorData.g / (double) rightColorData.r > GR_RATIO)
  {
    rightColorData.color = GREEN;
  }
  else if ((double) rightColorData.r / (double) rightColorData.g > RG_RATIO)
  {
    leftColorData.color = RED;
  }
  else
  {
    rightColorData.color = WHITE;
  }
  // Select left color sensor
  tcaselect(LEFT_COLOR);

  //Serial.println("Sensor Enter");
  // Get data and put it into the leftColorData struct
  color.getRawData(&leftColorData.r, &leftColorData.g, &leftColorData.b, &c);

  //Serial.println("Sensor Enter");
  //Serial.println(leftColorData.g);
  // Calculate the color that it was on (BLACK, WHITE OR GREEN)
  if (leftColorData.g < BVALUE * 1.5)
  {
    leftColorData.color = BLACK;
  }
  else if ((double) leftColorData.g / (double) leftColorData.r > GR_RATIO)
  {
    leftColorData.color = GREEN;
  }
  else if ((double) leftColorData.r / (double) leftColorData.g > RG_RATIO)
  {
    leftColorData.color = RED;
  }
  else
  {
    leftColorData.color = WHITE;
  }
  // Get TOF data

  //Serial.println("Sensor Enter");
  tcaselect(TOF);
  //Serial.println("Sensor Enter");
  
  TOFdistance = tof.readRangeContinuousMillimeters();
  //Serial.println("Sensor Enter");
  if (tof.timeoutOccurred())
  {
    Serial.print(" TIMEOUT");
    tof.init();
    tof.startContinuous();//Re-inits tof to fix timeout error
    loopSensors();
  }
  //Serial.println(TOFdistance);
  gyro.update();
  //Serial.print("\t X_Angle:");
  //Serial.print(gyro.getAngle(X_ANGLE));//X is based
}
/*
   Rotates the pixy cam and tof
*/
void rotatePixyCamAndTOF(float deg)
{
  pixy.setServos(90, 1000 / 180.0 * deg);
  delay(500);
  
}



#endif
