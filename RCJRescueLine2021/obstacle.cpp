#ifndef OBSTACLE_CPP
#define OBSTACLE_CPP
#include "obstacle.h"
#include "global.h"
#include "utils.h"
bool detectObstacles()
{
  if (TOFdistance < MAX_OBS_DIST) // seeing obstacle
  {
    forward(-((double)(MAX_OBS_DIST) - (double)TOFdistance) / 10.0, MOTOR_SPEED);
    rotatePixyCamAndTOF(180);
    loopSensors();
    tracingDir = TOFdistance < MAX_WALL_DIST; // looking for walls
    if (tracingDir) setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    else
    {
      rotatePixyCamAndTOF(0);
      setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    }
    loopSensors();
    while (TOFdistance > ROT_MAX_DIST)
    {
      loopMotors();
      loopSensors();
    }
    stopMotors();
    if (tracingDir) rotate(-90, MOTOR_SPEED); // getting ready to trace around obstacle
    else rotate(90, MOTOR_SPEED);
    return true;
  }
  return false;
}
bool doObstacles()
{
  forward(5, MOTOR_SPEED);
  if (tracingDir) rotate(-20, MOTOR_SPEED);
  else rotate(20, MOTOR_SPEED);
  loopSensors();
  while (rightColorData.g > (WVALUE / 1.5) && leftColorData.g > (WVALUE / 1.5))
  {
    loopSensors();
    loopMotors();
    double error = (double)TOFdistance - (double)(TARGET_DIST);
    if (error >= 100) error = 100;
    if (error <= -100) error = -100;
    double adj = (double)KP_OBS * error * (double)(tracingDir ? 1 : -1); // proportional rotation controller
    setMotorSpeeds(OBS_SPEED + adj, OBS_SPEED - adj);
  }
  stopMotors();
  rotatePixyCamAndTOF(90);
  forward(ROBOT_LENGTH / 2, MOTOR_SPEED);
  if (tracingDir)
  {
    setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    loopSensors();
    while (rightColorData.color == WHITE) // line detection
    {
      loopSensors();
      loopMotors();
    }
    stopMotors();
  }
  else
  {
    setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    loopSensors();
    while (leftColorData.color == WHITE) // line detection
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
