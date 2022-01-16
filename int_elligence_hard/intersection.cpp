#ifndef INTERSECTION_CPP
#define INTERSECTION_CPP
#include "global.h"
#include "utils.h"
#include "intersection.h"
/*
   Detects the intersection
*/
bool detectIntersection() {
  // On black line
  //if (follower.readSensors() != S1_OUT_S2_OUT) {
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
  Serial.println(rightGreen); Serial.println(leftGreen);
  if (leftGreen || rightGreen) {
    stopMotors();
    forward(1, 0.4);
    loopSensors();
    if ((leftColorData.color == WHITE && leftGreen) || (rightColorData.color == WHITE && rightGreen))
    {//If left sensor sees green 
      if (leftGreen)
      {
        rotateUntilLeftColorBlack();
      }
      else
      {
        rotateUntilRightColorBlack();
      }
      Serial.println("white");
      return false;
    }
    
    forward(ROBOT_LENGTH / 2, 0.31); //Move forward
    return true;
  }
  //}
  return false;
}
/*
   Rotates the robot until the left color sensor sees black
*/
void rotateUntilLeftColorBlack() {
  while (rightColorData.color != BLACK) {
    setMotorSpeeds(0.5, -0.5);
    loopMotors();
    loopSensors();
  }
  rotate(-10, 0.5);
}
/*
   Rotates the robot until the right color sensor sees black
*/
void rotateUntilRightColorBlack() {
  while (leftColorData.color != BLACK) {
    setMotorSpeeds(-0.5, 0.5);
    loopMotors();
    loopSensors();
  }
  rotate(10, 0.5);
}
/*
   Does the intersection
*/
bool doIntersection()
{
  // Forward for centering on line, so rotation works
  Serial.println("Centering on line");
  Serial.print(leftGreen);
  Serial.print('\t');
  Serial.println(rightGreen);

  if (leftGreen && rightGreen)
  {
    rotate(180, 0.31);
  }
  if (!leftGreen && rightGreen)
  {
    rotate(-90, 0.31);
    forward(3, 0.31);
    //rotateUntilRightColorBlack();
  }
  if (leftGreen && !rightGreen)
  {
    rotate(90, 0.31);
    forward(3, 0.31);
    //rotateUntilLeftColorBlack();
  }
  return false;
}

#endif
