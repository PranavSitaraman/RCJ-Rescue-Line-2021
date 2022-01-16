#ifndef INTERSECTION_CPP
#define INTERSECTION_CPP
#include "global.h"
#include "utils.h"
#include "intersection.h"
bool detectIntersection()
{
  loopSensors();
  leftGreen = (leftColorData.color == GREEN);
  rightGreen = (rightColorData.color == GREEN);
  if (leftGreen || rightGreen)
  {
    forward(1, 0.4);
  }
  else
  {
    return false;
  }
  leftGreen = (leftColorData.color == GREEN);
  rightGreen = (rightColorData.color == GREEN);
  if (leftGreen || rightGreen)
  {
    stopMotors();
    forward(1, 0.35);
    loopSensors();
    if ((leftColorData.color == WHITE && leftGreen) || (rightColorData.color == WHITE && rightGreen))
    {
      if (leftGreen)
      {
        rotateUntilLeftColorBlack();
      }
      else
      {
        rotateUntilRightColorBlack();
      }
      return false;
    }
    forward(ROBOT_LENGTH / 2, 0.31);
    return true;
  }
  return false;
}
void rotateUntilLeftColorBlack()
{
  while (rightColorData.color != BLACK)
  {
    setRawMotorSpeeds(0.5, -0.5);
    loopMotors();
    loopSensors();
  }
  rotate(-10, 0.5);
}
void rotateUntilRightColorBlack()
{
  while (leftColorData.color != BLACK)
  {
    setRawMotorSpeeds(-0.5, 0.5);
    loopMotors();
    loopSensors();
  }
  rotate(10, 0.5);
}
bool doIntersection()
{
  if (leftGreen && rightGreen)
  {
    rotate(180, 0.31);
  }
  if (!leftGreen && rightGreen)
  {
    rotate(-90, 0.31);
    forward(3, 0.31);
  }
  if (leftGreen && !rightGreen)
  {
    rotate(90, 0.31);
    forward(3, 0.31);
  }
  return false;
}
#endif
