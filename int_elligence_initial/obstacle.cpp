#ifndef OBSTACLE_CPP
#define OBSTACLE_CPP
#include "global.h"
#include "utils.h"
void obstacleavoidance()
{
  sensorvalues();
  if (ultrasonic_10.distanceCm() < forwardsensor)
  {
    resetmotors();
    turnDegrees('l', obstacleturn, reverseobstaclemotor, obstaclemotor);
    resetmotors();
    if (ultrasonic_10.distanceCm() < threshold)
    {
      resetmotors();
      turnDegrees('r', leftrightratio * obstacleturn, obstaclemotor, reverseobstaclemotor);
      resetmotors();
      g = 2;
    }
    if (g == 1)
    {
      LMotor.setTarPWM(outer);
      RMotor.setTarPWM(-inner);
    }
    else if (g == 2)
    {
      LMotor.setTarPWM(inner);
      RMotor.setTarPWM(-outer);
    }
    resetmotors();
    sensorvalues();
    while (xvalue > sensoraverage && avalue > sensoraverage)
    {
      RMotor.loop();
      LMotor.loop();
      if (g == 1)
      {
        LMotor.setTarPWM(outer);
        RMotor.setTarPWM(-inner);
      }
      else if (g == 2)
      {
        LMotor.setTarPWM(inner);
        RMotor.setTarPWM(-outer);
      }
      sensorvalues();
    }
    resetmotors();
    if (g == 1)
    {    
      while (sensorvalues() != 1)
      {
        turnDegrees('l', smallobstacleturn, 0, obstaclemotor);
      }
      turnDegrees('r', smallobstacleturn, obstaclemotor, 0);
    }
    else if (g == 2)
    {
      while (sensorvalues() != 2)
      {
        turnDegrees('r', smallobstacleturn, obstaclemotor, 0);
      }
      turnDegrees('l', smallobstacleturn, 0, obstaclemotor);
    }
    resetmotors();
  }
}
#endif
