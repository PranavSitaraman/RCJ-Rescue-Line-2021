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
void linefollow()
{
  uint16_t lv = leftColorData.g;
  uint16_t rv = rightColorData.g;
  double error = ((double)lv - (double)rv) / (WVALUE - BVALUE);
  double sum = prev_sum + error;
  double difference = error - prev_error;
  double adjusta = (double) error * KP + (double) sum * KI + (double) difference * KD;
  setRawMotorSpeeds((double) BASE_SPEED - adjusta, (double) BASE_SPEED + adjusta);
  prev_error = error;
  prev_sum += error;
}
#endif
