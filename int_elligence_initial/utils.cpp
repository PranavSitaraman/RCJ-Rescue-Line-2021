#ifndef UTILS_CPP
#define UTILS_CPP
#include "global.h"
/*
void _delay(float seconds)
{
  if (seconds < 0.0)
  {
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime)
  {
    RMotor.loop();
    LMotor.loop();
  }
}
*/
void isr_LMotorEncoder(void)
{
  if (digitalRead(LMotor.getPortB()) == 0)
  {
    LMotor.pulsePosMinus();
  }
  else
  {
    LMotor.pulsePosPlus();
  }
}
void isr_RMotorEncoder(void)
{
  if (digitalRead(RMotor.getPortB()) == 0)
  {
    RMotor.pulsePosMinus();
  }
  else
  {
    RMotor.pulsePosPlus();
  }
}
void turnDegrees(char q, float dist, int powB, int powC)
{
  if (q == 'l' && powC < 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (-Enc[rightMotor] > wheelbase/wheeldiameter * dist);
  }
  else if (q == 'l' && powC >= 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (-Enc[rightMotor] < wheelbase/wheeldiameter * dist);
  }
  else if (q == 'r' && powB < 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (Enc[leftMotor] > wheelbase/wheeldiameter * dist);
  }
  else if (q == 'r' && powB >= 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (Enc[leftMotor] < wheelbase/wheeldiameter * dist);
  }
  else if (q == 's' && powB < 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (Enc[leftMotor] > 360/(wheeldiameter * PI) * dist);
  }
  else if (q == 's' && powB >= 0)
  {
    do
    {
      RMotor.loop();
      LMotor.loop();
      LMotor.setTarPWM(powB);
      RMotor.setTarPWM(-powC);
      Enc[rightMotor] = RMotor.getCurPos();
      Pulse[rightMotor] = RMotor.getPulsePos();
      Enc[leftMotor] = LMotor.getCurPos();
      Pulse[leftMotor] = LMotor.getPulsePos();
    } while (Enc[leftMotor] < 360/(wheeldiameter * PI) * dist);
  }
}
int sensorvalues()
{
  int sensorState = lineFinder.readSensors();
  switch(sensorState)
  {
    case S1_IN_S2_IN: xvalue = 0; prevx = 0; avalue = 0; preva = 0; return 0;
    case S1_IN_S2_OUT: xvalue = 0; prevx = 0; avalue = 100; preva = 100; return 1;
    case S1_OUT_S2_IN: xvalue = 100; preva = 100; avalue = 0; preva = 0; return 2;
    case S1_OUT_S2_OUT: xvalue = 100; avalue = 100; return 3;
    default: return -1;
  }
}
void resetmotors()
{
  RMotor.setPulsePos(0);
  LMotor.setPulsePos(0);
  LMotor.setTarPWM(0);
  RMotor.setTarPWM(0);
}
#endif
