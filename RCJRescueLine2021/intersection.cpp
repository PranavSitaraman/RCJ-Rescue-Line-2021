#ifndef INTERSECTION_CPP
#define INTERSECTION_CPP
#include "global.h"
#include "utils.h"
#include "intersection.h"
bool detectIntersection()
{
  loopSensors();
  leftGreen = (leftColorData.color == GREEN); // green detect
  rightGreen = (rightColorData.color == GREEN);
  if (!leftGreen && !rightGreen) return false;
  forward(0.2, MOTOR_SPEED);
  loopSensors();
  leftGreen = (leftColorData.color == GREEN);
  rightGreen = (rightColorData.color == GREEN);
  if (!leftGreen && !rightGreen) return false;
  forward(3, MOTOR_SPEED);
  loopSensors();
  if ((leftColorData.color == WHITE && leftGreen) || (rightColorData.color == WHITE && rightGreen)) // line check
  {
    if (leftGreen && !rightGreen) rotateUntilLeftColorBlack();
    else if (rightGreen && !leftGreen) rotateUntilRightColorBlack();
    return false;
  }
  forward(ROBOT_LENGTH / 4, MOTOR_SPEED);
  return true;
}
void rotateUntilLeftColorBlack()
{
  while (rightColorData.color != BLACK)
  {
    setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    loopMotors();
    loopSensors();
  }
  rotate(-20, MOTOR_SPEED);
}
void rotateUntilRightColorBlack()
{
  while (leftColorData.color != BLACK)
  {
    setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    loopMotors();
    loopSensors();
  }
  rotate(20, MOTOR_SPEED);
}
void rotateUntilLeftColorBlack2()
{
  while (leftColorData.color != BLACK)
  {
    setMotorSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    loopMotors();
    loopSensors();
  }
  rotate(20, MOTOR_SPEED);
}
void rotateUntilRightColorBlack2()
{
  while (rightColorData.color != BLACK)
  {
    setMotorSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    loopMotors();
    loopSensors();
  }
  rotate(-20, MOTOR_SPEED);
}
bool doIntersection()
{
  if (leftGreen && rightGreen) rotate(180, MOTOR_SPEED);
  if (!leftGreen && rightGreen) { rotate(-60, MOTOR_SPEED); rotateUntilRightColorBlack2(); }
  if (leftGreen && !rightGreen) { rotate(60, MOTOR_SPEED); rotateUntilLeftColorBlack2(); }
  forward(5, MOTOR_SPEED);
  return false;
}
#endif
