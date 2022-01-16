#include "global.h"
#include "utils.h"
#include "linetracing.h"
#include "intersection.h"
#include "obstacle.h"
#include "evac.h"
MePort_Sig mePort[17] = { {NC, NC}, {5, 4}, {3, 2}, {7, 6}, {9, 8}, {16, 17}, {A10, A15}, {A9, A14}, {A8, A13}, {A7, A12}, {A6, A11}, {NC, A2}, {NC, A3}, {NC, A0}, {NC, A1}, {NC, NC}, {NC, NC} };
Encoder_port_type encoder_Port[6] = { {NC, NC, NC, NC, NC}, {19, 42, 11, 49, 48}, {18, 43, 10, 47, 46}, {NC, NC, NC, NC, NC}, {NC, NC, NC, NC, NC} };
Servo ServoClaw; Pixy2 pixy; VL53L0X tof; Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
double prev_sum = 0, prev_error = 0;
int turn = 0, maxsilvercount = 0, maxgreencount = 0, yoffset = 95, xoffset = 5, cube_x, cube_y, rescueSide = 0, lEnc = 0, rEnc = 0, silverCountTape = 0;
bool tracingDir = false, leftGreen = false, rightGreen = false, leftBlack = false, rightBlack = false, frontBlack = false;
double GR_RATIO = 1.5, WVALUE = 1000, SKEW_RIGHT = 1, BVALUE = 60, lTarPwm = 0, lCurPwm = 0, rTarPwm = 0, rCurPwm = 0;
ColorData rightColorData, leftColorData, centerColorData;
uint16_t TOFdistance = 0, TOFdistance_GROUND = 0;
long prevLoopMillis = 0;
enum State { LINE_FOLLOW, INTERSECTIONS, OBSTACLES, EVAC_ROOM, GAP, END };
State state = LINE_FOLLOW;
void setup()
{
  Serial.begin(9600); Wire.begin();
  Serial.println("start");
  if (!initRobot())
  {
    Serial.println("fail"); while (1); // initialization
  }
  Serial.println("success");
  calibration(); // calibration
}
void loop()
{
  loopMotors();
  loopSensors();
  switch (state)
  {
    case LINE_FOLLOW: // line tracing
      linefollow(NORM_LINE_SPEED);
      if (rightColorData.color == RED || leftColorData.color == RED) state = END;
      else if (detectEVAC()) state = EVAC_ROOM;
      else if (detectObstacles()) state = OBSTACLES;
      else if (detectIntersection()) state = INTERSECTIONS;
      break;
    case INTERSECTIONS: // intersections
      if (!(doIntersection()))
      {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case OBSTACLES: // obstacle avoidance
      if (!(doObstacles()))
      {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case EVAC_ROOM: // evacuation zone
      doEvac();
      state = LINE_FOLLOW;
      resetLineFollowVariables();
      break;
    case END: // red tape
      stopMotors();
      break;
  }
}
