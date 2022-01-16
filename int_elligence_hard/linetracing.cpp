#ifndef LINETRACING_CPP
#define LINETRACING_CPP
#include "global.h"
#include "utils.h"
void resetLineFollowVariables()
{
    prev_error = 0;
    prev_sum = 0;
}
void gap()
{
  setMotorSpeeds(0.6, 0.6);
  while(follower.readSensors() == S1_OUT_S2_OUT)
  {
    loopMotors();
  }
  stopMotors();
}
void linefollow()
{
    uint16_t lv = leftColorData.g;  //left green val
    uint16_t rv = rightColorData.g; //right green val
    // Calculate Error and then normalize by dividing the max difference (WVALUE - BVALUE)
    double error = ((double)lv - (double)rv * 1.2) / (WVALUE-BVALUE);
    //Serial.print("Error: ");
    //Serial.print(error);
    double sum = prev_sum + error;
    double difference = error - prev_error;
    double adjusta = (double) error * KP + (double) sum * KI + (double) difference * KD;

    //Serial.print("\t Adjustment: ");
    //Serial.println(adjusta);
    // We use raw speed here because it makes the motors respond much faster and we are using PID, so we dont need an extra layer of PID.
    //However this seems to result in a substantially greater speed. Despite this drawback, it is noted that robust KP,KI, and KD values
    //mitigate detriments provided by this higher speed. 
    setMotorSpeeds((double) BASE_SPEED - adjusta - adjusta/3, (double) BASE_SPEED + adjusta - adjusta/3);
    prev_error = error;
    prev_sum += error;
    if (fabs(prev_sum) > 5000)
    {
      resetLineFollowVariables();
    }
    if (follower.readSensors() == S1_OUT_S2_OUT && leftColorData.color == WHITE && rightColorData.color == WHITE)
    {
      resetLineFollowVariables();
    }
}
#endif
