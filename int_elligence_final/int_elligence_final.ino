#include "global.h"
#include "utils.h"
#include "linetracing.h"
#include "intersection.h"
#include "evac.h"

MePort_Sig mePort[17] =
{
  {NC, NC},
  {5, 4},
  {3, 2},
  {7, 6},
  {9, 8},
  {16, 17},
  {A10, A15},
  {A9, A14},
  {A8, A13},
  {A7, A12},
  {A6, A11},
  {NC, A2},
  {NC, A3},
  {NC, A0},
  {NC, A1},
  {NC, NC},
  {NC, NC},
};
Encoder_port_type encoder_Port[6] =
{
  {NC, NC, NC, NC, NC},
  {19, 42, 11, 49, 48},
  {18, 43, 10, 47, 46},
  {NC, NC, NC, NC, NC},
  {NC, NC, NC, NC, NC},
};
int servoPin = 44;
Servo ServoClaw;
Pixy2 pixy;
MeEncoderOnBoard leftm(SLOT1);
MeEncoderOnBoard rightm(SLOT2);
VL53L0X tof;
Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
MeLineFollower follower(PORT_6);

double prev_sum = 0;
double prev_error = 0;
int turn = 0;

bool leftGreen = false;
bool rightGreen = false;
bool leftBlack = false;
bool rightBlack = false;
bool frontBlack = false;
double GR_RATIO = 1.5;//the green to red ratio
double WVALUE = 1000;//White's avg value

int yoffset =  95;
int xoffset =  5;
int cube_x;
int cube_y;


double BVALUE = 60;//Black's avg value

ColorData rightColorData;
ColorData leftColorData;//
uint16_t TOFdistance = 0;//Variable to hold Time Of Flight difference

enum State { EVAC_ROOM, LINE_FOLLOW, INTERSECTIONS, OBSTACLES };

State state = /*LINE_FOLLOW*/EVAC_ROOM;
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  initRobot();
  resetLineFollowVariables();
  Serial.println(LINE_FOLLOW);
  Serial.println(state);
  pixy.init();
  pixy.changeProg("video");
  clawInit();
  pixyInit();
}
void loop()
{
  loopSensors();
  loopMotors();
  switch (state)
  {
    case LINE_FOLLOW:

      linefollow();

      if (detectIntersection()) {
        state = INTERSECTIONS;
      }
      break;
    case INTERSECTIONS:
      if (!doIntersection()) {
        state = LINE_FOLLOW;
      }
      break;
    case OBSTACLES: break;
    case EVAC_ROOM:
    //Move back 5 cm before starting: allows to see the most distance. Only do this when see silver. 
    if(FindSilver())
    {
      if(cube_x > 165)
      {
        turn = 1;
      }
      else if(cube_x < 165)
      {
        turn = -1;
      }
      clawRaise();
      Align2Object(turn);
      Move2Object();
      clawRelease();
      clawLower();
      clawClamp();
      clawRaise();
      //search for black rescue zone)
      delay(100000000000);
    }
    break;
  }
}
