#ifndef LINETRACING_CPP
#define LINETRACING_CPP
#include "global.h"
#include "utils.h"
void linetracing()
{
  error = xvalue - avalue - tar;
  integral = integral + error;
  if (error < derivativesetpoint)
  {
    derivative = derivativesetpoint - (fabs(error - lastError))/2;
  }
  else
  {
    derivative = (fabs(error - lastError))/2;
  }
  turn = Kp * error + Ki * integral + Kd * derivative;
  powerB = motortarget + turn;
  powerC = motortarget - turn;
  LMotor.setTarPWM(powerB);
  RMotor.setTarPWM(-powerC);
  lastError = error;
  if (integral > integraluplimit)
  {
    integral = integraltarget;
  }
  else if (integral < integraldownlimit)  
  {
    integral = integraltarget;
  }
}
#endif
