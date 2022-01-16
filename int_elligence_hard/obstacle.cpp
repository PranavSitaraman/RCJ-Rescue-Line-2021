#ifndef OBSTACLE_CPP
#define OBSTACLE_CPP
#include "obstacle.h"
#include "global.h"
#include "utils.h"

bool detectObstacles()
{
  if (TOFdistance < MAX_OBS_DIST /*mm or 5 cm*/)
  {
    stopMotors();
    forward(-((double)(MAX_OBS_DIST) - (double)TOFdistance)/10.0, 0.4);
    rotatePixyCamAndTOF(180);
    Serial.println("!!!");
    loopSensors();
    Serial.println("???");
    tracingDir = TOFdistance < MAX_WALL_DIST;
    if (tracingDir) {
      Serial.println("right");
      setMotorSpeeds(-0.5, 0.5);
    } else
    {
      Serial.println("left");
      rotatePixyCamAndTOF(0);
      delay(1000);
      setMotorSpeeds(0.5, -0.5);
    }
    loopSensors();
    while (TOFdistance > ROT_MAX_DIST) {
      loopMotors();
      loopSensors();
    }
    stopMotors();
    Serial.println("adjust turn start");
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

bool doObstacles() {
  forward(5, 0.3);
  if (tracingDir)
  {
    rotate(10, 0.5);
  }
  else
  {
    rotate(10, 0.5);
  }
  loopSensors();
  while (rightColorData.g > (WVALUE/2) && leftColorData.g > (WVALUE/2)) {
    loopSensors();
    loopMotors();
    double error = (double)TOFdistance - (double)(TARGET_DIST);
    if(error >= 100){error = 100;}
    double adj = (double)KP_OBS * error * (double)(tracingDir ? 1 : -1);//
    setMotorSpeeds(BASE_SPEED + adj, BASE_SPEED - adj);
    //Serial.print(rightColorData.g);
    Serial.print((WHITE/2));
    Serial.print("\t");
    
    Serial.print(leftColorData.g > (WHITE/2));
    Serial.print("\t");
    
    Serial.print(rightColorData.g > (WHITE/2));
    Serial.print("\t");
    Serial.print(TOFdistance);
    Serial.print("\t Adjust: ");
    Serial.print(adj);
    Serial.print("\t Target Distance: ");
    Serial.print(TARGET_DIST);
    Serial.print("\t Error: ");
    Serial.println(error);
  }
  stopMotors();
  rotatePixyCamAndTOF(90);
  delay(1000);
  forward(ROBOT_LENGTH/2, 0.4);
  delay(1000);
  Serial.println(tracingDir);//0
  if(tracingDir)
  {//turn right
    setMotorSpeeds(-0.5, 0.5);
    loopSensors();
    while(leftColorData.color == WHITE)
    {
      Serial.print("Left:");
      Serial.println(leftColorData.color);
      loopSensors();
      loopMotors();
    }
    stopMotors();
  }
  else
  {//turn left
    Serial.println("ENTER <3");
    setMotorSpeeds(0.5, -0.5);
    loopSensors();
    while(rightColorData.color == WHITE)
    {
      Serial.print("Right:");
      Serial.println(rightColorData.color);
      loopSensors();
      loopMotors();
    }
    stopMotors();
  }
  Serial.println("Done!");
  rotatePixyCamAndTOF(90);
  return false;
}
#endif
