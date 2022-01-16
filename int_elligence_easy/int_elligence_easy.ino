#include "global.h"
#include "utils.h"
#include "linetracing.h"
#include "intersection.h"
#include "obstacle.h"
#include "evac.h"
MePort_Sig mePort[17] = { {NC, NC}, {5, 4}, {3, 2}, {7, 6}, {9, 8}, {16, 17}, {A10, A15}, {A9, A14}, {A8, A13}, {A7, A12}, {A6, A11}, {NC, A2}, {NC, A3}, {NC, A0}, {NC, A1}, {NC, NC}, {NC, NC} };
Encoder_port_type encoder_Port[6] = { {NC, NC, NC, NC, NC}, {19, 42, 11, 49, 48}, {18, 43, 10, 47, 46}, {NC, NC, NC, NC, NC}, {NC, NC, NC, NC, NC} };
Servo ServoClaw;
Pixy2 pixy;
VL53L0X tof;
Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
MeGyro gyro(0, 0x69);
double prev_sum = 0;
double prev_error = 0;
int turn = 0;
bool tracingDir = false;
bool leftGreen = false;
bool rightGreen = false;
bool leftBlack = false;
bool rightBlack = false;
bool frontBlack = false;
double GR_RATIO = 1.75;
double WVALUE = 1000;
int yoffset =  95;
int xoffset =  5;
int cube_x;
int cube_y;
double BVALUE = 60;
ColorData rightColorData;
ColorData leftColorData;
uint16_t TOFdistance = 0;
int rescueSide = 0;
double lTarPwm = 0;
double lCurPwm = 0;
double rTarPwm = 0;
double rCurPwm = 0;
int lEnc = 0;
int rEnc = 0;
long prevLoopMillis = 0;
enum State { LINE_FOLLOW, INTERSECTIONS, OBSTACLES, EVAC_ROOM, END };
State state = LINE_FOLLOW;
void setup()
{
  Wire.begin();
  initRobot();
  int suml = 0;
  int sumr = 0;
  for (int i = 0; i < 30; i++)
  {
    loopSensors();
    suml += leftColorData.g;
    sumr += rightColorData.g;
  }
  BVALUE = suml / 30;
  WVALUE = sumr / 30;
}
void loop()
{
  loopMotors();
  switch (state)
  {
    case LINE_FOLLOW:
      linefollow();
      if (detectIntersection())
      {
        state = INTERSECTIONS;
      }
      if (detectObstacles())
      {
        state = OBSTACLES;
      }
      break;
    case INTERSECTIONS:
      if (!doIntersection())
      {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case OBSTACLES:
      if (!doObstacles())
      {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case EVAC_ROOM:
      if (FindSilver())
      {
        doSilver();
        state = LINE_FOLLOW;
      }
      break;
    case END:
      stopMotors();
      break;
  }
  loopSensors();
  if (rightColorData.color == RED || leftColorData.color == RED)
  {
    state = END;
  }
}
