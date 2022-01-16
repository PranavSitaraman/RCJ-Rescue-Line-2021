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
    uint16_t lv = leftColorData.g;//left green val
    uint16_t rv = rightColorData.g;//right green val
    double error = ((double)lv - (double)rv) / (WVALUE-BVALUE);
    Serial.print("Error: ");
    Serial.print(error);
    double sum = prev_sum + error;
    double difference = error - prev_error;
    double adjusta = (double) error * KP + (double) sum * KI + (double) difference * KD;
    Serial.print("\t Adjustment: ");
    Serial.print(adjusta);
    // We use raw speed here because it makes the motors respond much faster and we are using PID, so we dont need an extra layer of PID.
    // However this seems to result in a substantially greater speed. Despite this drawback, it is noted that robust KP,KI, and KD values
    // mitigate detriments provided by this higher speed.
    setMotorSpeeds((double) BASE_SPEED - adjusta, (double) BASE_SPEED + adjusta, true);
    prev_error = error;
    prev_sum += error;
    Serial.print("\t Left Cur Pos: ");
    Serial.println(leftm.getCurPos());
    if(follower.readSensors() == S1_OUT_S2_OUT)
    {
      //gap();
    }
}
#endif
