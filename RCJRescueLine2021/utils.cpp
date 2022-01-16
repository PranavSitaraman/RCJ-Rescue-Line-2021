#ifndef UTILS_CPP
#define UTILS_CPP
#include "global.h"
#include "utils.h"
#include "linetracing.h"
void ClawRaise()
{
  ServoClaw.write(100);
}
void ClawLower()
{
  ServoClaw.write(0);
}
void ClawVibration()
{
  int ClawPosition = ServoClaw.read();
  for (int i = 0; i < 10; i++)
  {
    ServoClaw.write(ClawPosition+20);
    ServoClaw.write(ClawPosition-20);
  }
}
void ClawRelease()
{
  ServoClaw.write(180);
}
void ClawClamp()
{
  for (int x = 720; x >= 0; x--)
  {
    pixy.setServos(450, x);
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
int tcaselect(uint8_t i) // selecting channel on mux
{
  if (i > 7) return -4;
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  return (Wire.endTransmission());
}
bool initRobot() // initializing the robot
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
  tcaselect(RIGHT_COLOR);
  if (!color.begin())
  {
    Serial.println("right color"); return false;
  }
  tcaselect(LEFT_COLOR);
  if (!color.begin())
  {
    Serial.println("left color"); return false;
  }
  tcaselect(TOF);
  tof.setTimeout(500);
  if (!tof.init())
  {
    Serial.println("TOF"); return false;
  }
  tof.startContinuous();
  pixy.init();
  pixy.changeProg("video");
  ServoClaw.attach(SERVO_PIN);
  ServoClaw.write(95);
  return true;
}
void setMotorSpeeds(double left, double right) // turning movement
{
  if (fabs(left) >= 1) left = copysign(1, left);
  if (fabs(right) >= 1) right = copysign(1, right);
  if (fabs(left) < MOTOR_DEADZONE)
  {
    if (fabs(left) < MOTOR_DEADZONE / 5) left = 0;
    else left = copysign(MOTOR_DEADZONE, left);
  }
  if (fabs(right) < MOTOR_DEADZONE)
  {
    if (fabs(right) < MOTOR_DEADZONE / 5) right = 0;
    else right = copysign(MOTOR_DEADZONE, right);
  }
  lTarPwm = left;
  rTarPwm = right;
}
void setRawMotorSpeeds(double left, double right) // PID movement
{
  lCurPwm = left;
  if (left < 0)
  {
    analogWrite(lMotorPwm, -left * 180);
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
  }
  else
  {
    analogWrite(lMotorPwm, left * 180);
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
  }
  rCurPwm = right;
  if (right < 0)
  {
    analogWrite(rMotorPwm, -right * 180);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
  }
  else
  {
    analogWrite(rMotorPwm, right * 180);
    digitalWrite(rMotor1, HIGH);
    digitalWrite(rMotor2, LOW);
  }
}
void loopMotors()
{
  double lChange = lTarPwm - lCurPwm;
  double rChange = rTarPwm - rCurPwm;
  long curLoopMillis = millis();
  double maxChange = MAX_ACC * ((curLoopMillis - prevLoopMillis) / 1000.0);
  setRawMotorSpeeds(lCurPwm + constrain(lChange, -maxChange, maxChange), rCurPwm + constrain(rChange, -maxChange, maxChange));
  prevLoopMillis = curLoopMillis;
}
void stopMotors()
{
  setMotorSpeeds(0, 0);
  lTarPwm = 0;
  rTarPwm = 0;
}
void moveTo(int ltarget, int rtarget, double speed) // generic movement function
{
  stopMotors();
  lEnc = 0;
  rEnc = 0;
  loopMotors();
  setMotorSpeeds(copysign(speed, ltarget), copysign(speed, rtarget));
  loopMotors();
  while (abs(lEnc) < abs(ltarget) || abs(rEnc) < abs(rtarget))
  {
    Serial.println(abs(lEnc));
    Serial.println(abs(rEnc));
    loopMotors();
  }
  stopMotors();
}
void forward(double dist, double speed)
{
  moveTo(dist * (ENC_PER_CM), dist * (ENC_PER_CM), speed);
}
void rotate(double angle, double speed)
{
  moveTo(angle * ENC_PER_DEG, -angle * ENC_PER_DEG, speed);
}
void loopSensors() // reading from color sensors and time of flight
{
  uint16_t c;
  tcaselect(RIGHT_COLOR);
  color.getRawData(&rightColorData.r, &rightColorData.g, &rightColorData.b, &c);
  rightColorData.temp = color.calculateColorTemperature(rightColorData.r, rightColorData.g, rightColorData.b);
  rightColorData.r *= SKEW_RIGHT;
  rightColorData.g *= SKEW_RIGHT;
  rightColorData.b *= SKEW_RIGHT;
  if (rightColorData.g < BVALUE * 1.5) rightColorData.color = BLACK;
  else rightColorData.color = WHITE;
  if (((double) rightColorData.g / (double) rightColorData.r) > GR_RATIO) rightColorData.color = GREEN;
  if ((double) rightColorData.r / (double) rightColorData.g > RG_RATIO) leftColorData.color = RED;
  tcaselect(LEFT_COLOR);
  color.getRawData(&leftColorData.r, &leftColorData.g, &leftColorData.b, &c);
  leftColorData.temp = color.calculateColorTemperature(leftColorData.r, leftColorData.g, leftColorData.b);
  if (leftColorData.g < BVALUE * 1.5) leftColorData.color = BLACK;
  else leftColorData.color = WHITE;
  if (((double) leftColorData.g / (double) leftColorData.r) > GR_RATIO) leftColorData.color = GREEN;
  if ((double) leftColorData.r / (double) leftColorData.g > RG_RATIO) leftColorData.color = RED;
  color.getRawData(&centerColorData.r, &centerColorData.g, &centerColorData.b, &c);
  centerColorData.temp = color.calculateColorTemperature(centerColorData.r, centerColorData.g, centerColorData.b);
  if (centerColorData.g > WVALUE * 1.1) centerColorData.color = WHITE;
  else centerColorData.color = BLACK;
  tcaselect(TOF);
  TOFdistance = tof.readRangeContinuousMillimeters();
  if (tof.timeoutOccurred())
  {
    tof.init();
    tof.startContinuous();
    loopSensors();
  }
}
void rotatePixyCamAndTOF(float deg)
{
  pixy.setServos(1000 / 180 * deg, 1000 / 180.0 * deg);
}
void calibration() // initial field calibration
{
  loopSensors();
  double leftb = 1024;
  double leftw = leftColorData.g;
  double rightb = 1024;
  double rightw = rightColorData.g;
  setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
  int numFailChecks = 0;
  while (numFailChecks < 3)
  {
    loopMotors();
    loopSensors();
    if (leftb > leftColorData.g) leftb = leftColorData.g;
    else if (leftColorData.g < leftw * 0.75)
    {
      numFailChecks++;
      stopMotors();
      setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    }
  }
  rightw = rightColorData.g;
  numFailChecks = 0;
  stopMotors();
  setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
  while (numFailChecks < 3)
  {
    loopMotors();
    loopSensors();
    if (rightb > rightColorData.g) rightb = rightColorData.g;
    else if (rightColorData.g < rightw * 0.75)
    {
      numFailChecks++;
      stopMotors();
      setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    }
  }
  leftw = leftColorData.g;
  stopMotors();
  SKEW_RIGHT = leftw / rightw;
  BVALUE = leftb;
  WVALUE = leftw;
  rotate(-20, MOTOR_SPEED);
}
#endif
