#ifndef OBSTACLE_CPP
#define OBSTACLE_CPP
#include "global.h"
#include "utils.h"
#include "obstacle.h"
bool detectObstacles()
{
  if (TOFdistance < MAX_OBS_DIST)
  {
    stopMotors();
    forward(-((double)(MAX_OBS_DIST) - (double)TOFdistance) / 10.0, 0.35);
    rotatePixyCamAndTOF(180);
    loopSensors();
    tracingDir = TOFdistance < MAX_WALL_DIST;
    if (tracingDir)
    {
      setRawMotorSpeeds(-0.3, 0.3);
    }
    else
    {
      rotatePixyCamAndTOF(0);
      setRawMotorSpeeds(0.3, -0.3);
    }
    loopSensors();
    while (TOFdistance > ROT_MAX_DIST)
    {
      loopMotors();
      loopSensors();
    }
    stopMotors();
    if (tracingDir)
    {
      rotate(-20, 0.5);
    }
    else
    {
      rotate(20, 0.5);
    }
    stopMotors();
    return true;
  }
  return false;
}
bool doObstacles()
{
  forward(5, 0.3);
  if (tracingDir)
  {
    rotate(-10, 0.5);
  }
  else
  {
    rotate(10, 0.5);
  }
  loopSensors();
  while (rightColorData.g > (WVALUE / 1.5) && leftColorData.g > (WVALUE / 1.5))
  {
    loopSensors();
    loopMotors();
    double error = (double)TOFdistance - (double)(TARGET_DIST);
    if (error >= 100)
    {
      error = 100;
    }
    double adj = (double)KP_OBS * error * (double)(tracingDir ? 1 : -1);//
    setRawMotorSpeeds(BASE_SPEED + adj, BASE_SPEED - adj);
  }
  stopMotors();
  rotatePixyCamAndTOF(90);
  forward(ROBOT_LENGTH / 2, 0.4);
  if (tracingDir)
  {
    setRawMotorSpeeds(-0.5, 0.5);
    loopSensors();
    while (leftColorData.color == WHITE)
    {
      loopSensors();
      loopMotors();
    }
    stopMotors();
  }
  else
  {
    setRawMotorSpeeds(0.5, -0.5);
    loopSensors();
    while (rightColorData.color == WHITE)
    {
      loopSensors();
      loopMotors();
    }
    stopMotors();
  }
  rotatePixyCamAndTOF(90);
  return false;
}
#endif
