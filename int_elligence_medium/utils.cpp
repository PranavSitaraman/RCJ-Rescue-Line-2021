#ifndef UTILS_CPP
#define UTILS_CPP
#include "global.h"
#include "utils.h"
void ClawRaise()
{
  ServoClaw.write(90);
}
void ClawLower()
{
  ServoClaw.write(0);
}
void ClawVibration()
{
  ServoClaw.write(60);
  for (int i = 0; i < 10; i++)
  {
    ServoClaw.write(60);
    ServoClaw.write(40);
  }
}
void ClawRelease()
{
  for (int x = 0; x <= 720; x++)
  {
    pixy.setServos(450, x);
  }
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
int tcaselect(uint8_t i)
{
  if (i > 7)
  {
    return -4;
  }
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  return (Wire.endTransmission());
}
void initRobot()
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
  tcaselect(LEFT_COLOR);
  tcaselect(TOF);
  tof.setTimeout(500);
  tof.startContinuous();
  pixy.init();
  pixy.changeProg("video");
  ServoClaw.attach(SERVO_PIN);
  ServoClaw.write(180);
  gyro.begin();
}
void setRawMotorSpeeds(double left, double right)
{
  lCurPwm = left;
  if (left < 0)
  {
    analogWrite(lMotorPwm, -left * 200);
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
  }
  else
  {
    analogWrite(lMotorPwm, left * 200);
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
  }
  rCurPwm = right;
  if (right < 0)
  {
    analogWrite(rMotorPwm, -right * 200);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
  }
  else
  {
    analogWrite(rMotorPwm, right * 200);
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
  setRawMotorSpeeds(0, 0);
  lTarPwm = 0;
  rTarPwm = 0;
}
void moveTo(int ltarget, int rtarget, double speed)
{
  lEnc = 0;
  rEnc = 0;
  loopMotors();
  setRawMotorSpeeds(copysign(speed, ltarget), copysign(speed, rtarget));
  Serial.print(ltarget);
  Serial.print("\t");
  Serial.println(rtarget);
  while (abs(lEnc) < abs(ltarget) || abs(rEnc) < abs(rtarget))
  {
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
void loopSensors()
{
  uint16_t c;
  tcaselect(RIGHT_COLOR);
  color.getRawData(&rightColorData.r, &rightColorData.g, &rightColorData.b, &c);
  rightColorData.temp = color.calculateColorTemperature(rightColorData.r, rightColorData.g, rightColorData.b);
  rightColorData.r *= SKEW_RIGHT;
  rightColorData.g *= SKEW_RIGHT;
  rightColorData.b *= SKEW_RIGHT;
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
  tcaselect(LEFT_COLOR);
  color.getRawData(&leftColorData.r, &leftColorData.g, &leftColorData.b, &c);
  leftColorData.temp = color.calculateColorTemperature(leftColorData.r, leftColorData.g, leftColorData.b);
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
  tcaselect(TOF);
  TOFdistance = tof.readRangeContinuousMillimeters();
  if (tof.timeoutOccurred())
  {
    tof.init();
    tof.startContinuous();
    loopSensors();
  }
  gyro.update();
}
void rotatePixyCamAndTOF(float deg)
{
  pixy.setServos(1000 / 180 * deg, 1000 / 180.0 * deg);
  delay(500);
}
#endif
