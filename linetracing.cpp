#ifndef LINETRACING_CPP
#define LINETRACING_CPP
#include "global.h"
#include "utils.h"
#include "linetracing.h"
void resetLineFollowVariables()
{
  prev_error = 0;
  prev_sum = 0;
}
double linefollow(double speed)
{
  uint16_t lv = constrain(leftColorData.g, BVALUE, WVALUE), rv = constrain(rightColorData.g, BVALUE, WVALUE);
  double error = ((double)lv - (double)rv) / (WVALUE - BVALUE);
  double sum = prev_sum + error;
  double difference = error - prev_error;
  double adjusta = (double)error * KP + (double)sum * KI + (double)difference * KD; // PID adjustment
  Serial.print(leftColorData.temp);
  Serial.print("\t");
  Serial.println(rightColorData.temp);
  setMotorSpeeds((double)speed - adjusta, (double)speed + adjusta);
  prev_error = error;
  prev_sum += error;
  return error;
}
#endif
