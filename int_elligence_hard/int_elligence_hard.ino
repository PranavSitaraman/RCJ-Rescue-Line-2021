#include "global.h"
#include "utils.h"
#include "linetracing.h"
#include "intersection.h"
#include "obstacle.h"
#include "evac.h"

// REFER TO global.h FOR COMMENTS
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
Servo ServoClaw;
Pixy2 pixy;
MeEncoderOnBoard leftm(SLOT1);
MeEncoderOnBoard rightm(SLOT2);
VL53L0X tof;
Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
MeLineFollower follower(PORT_6);
MeGyro gyro(0, 0x69);
MeBuzzer buzzer;

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

// This is the state enum so it has all of the main states
enum State { LINE_FOLLOW, INTERSECTIONS, OBSTACLES, EVAC_ROOM, END };
State state = LINE_FOLLOW;


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  bool x = initRobot();
  if (!x)
  {
    while (1) {
      //Serial.println("FAIL");
    }
  }
  //Serial.println(x);
  calibration();
  rotate(20, 0.3);
}
void loop()
{
  switch (state)
  {
    case LINE_FOLLOW:
      Serial.println("Line Following");
      linefollow();

      if (detectIntersection()) {
        state = INTERSECTIONS;
      }
      if (detectObstacles()) {
        state = OBSTACLES;
      }
      break;
    case INTERSECTIONS:
      //Serial.println("Intersections");
      if (!doIntersection()) {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case OBSTACLES:
      //Serial.println("Obstacles");
      if (!doObstacles()) {
        state = LINE_FOLLOW;
        resetLineFollowVariables();
      }
      break;
    case EVAC_ROOM:
      //Serial.println("Evacuation Room");
      //Move back 5 cm before starting: allows to see the most distance. Only do this when see silver.
      if (FindSilver())
      {
        if (cube_x > 165)
        {
          turn = 1;
        }
        else if (cube_x < 165)
        {
          turn = -1;
        }
        ClawRaise();
        float degreesTurned = Align2Object(turn);
        float distanceMoved = Move2Object();
        ClawRelease();
        ClawLower();
        ClawClamp();
        ClawRaise();
        forward(-distanceMoved, 85);
        rotate(-degreesTurned, 100);
        loopSensors();
        float TOF_holder = TOFdistance;
        while (TOFdistance > TOF_holder / 2)
        {
          loopSensors();
          setMotorSpeeds(0.35, 0.35);
          loopMotors();
        }
        stopMotors();
        switch (RescueZoneSearch())
        {
          case FRONT_CORNER:
          case RIGHT_CORNER:
            rotate(-45, 0.4);
          case LEFT_CORNER:
            rotate(-135, 0.4);
          case LEFT_CORNER*2:
            rotate(45, 0.4);
          case RIGHT_CORNER*2:
            rotate(135, 0.4);
        }
        while (TOFdistance > MAX_OBS_DIST)
        {
          loopSensors();
          setMotorSpeeds(0.35, 0.35);
          loopMotors();
        }
        ClawLower();
        ClawRelease();
        rotate(180, 0.5);

        int holding = 0;
        int holding_Two = 0;
        for (int x = 0; x <= 180; x++)
        {
          rotatePixyCamAndTOF(x);
          loopSensors();
          if (TOFdistance > holding)
          {
            holding = TOFdistance;
            holding_Two = x;
          }
          if (x == 180)
          {
            if (holding_Two > 90)
            {
              rotate(-(holding_Two - 90), 0.35);
            }
            else if (holding_Two < 90)
            { //left
              rotate((90 - holding_Two), 0.35);
            }
            rotatePixyCamAndTOF(90);
            while (TOFdistance > MAX_OBS_DIST)
            {
              loopSensors();
              setMotorSpeeds(0.35, 0.35);
              loopMotors();
            }
          }
        }
        forward(4, 0.4);
        setMotorSpeeds(-0.35, 0.35);
        loopSensors();
        while (rightColorData.color != GREEN && leftColorData.color != GREEN)
        {
          loopMotors();
          loopSensors();
        }
        stopMotors();
        setMotorSpeeds(-0.35, 0.35);
        loopSensors();
        while (rightColorData.color != BLACK || leftColorData.color != BLACK)
        {
          loopMotors();
          loopSensors();
        }
        stopMotors();
        state = LINE_FOLLOW;
      }
      break;
      case END:
        stopMotors();
      break;
  }
  loopSensors();
  loopMotors();
  if(rightColorData.color == RED || leftColorData.color == RED) {
    state = END;
  }
}

void calibration() {
  Serial.println("-----BATTERY-LEVEL-----");
  Serial.println(analogRead(BATTERY_PIN));

  Serial.println("------CALIBRATING------");
  avgColor(&BVALUE, &WVALUE);

  Serial.println("---DONE--CALIBRATING---");
  Serial.print("BLACK:");
  Serial.println(BVALUE);
  Serial.print("WHITE:");
  Serial.println(WVALUE);
}

void avgColor(double* left, double* right) {
  int suml = 0;
  int sumr = 0;
  double n = 30;
  for (int i = 0; i < n; i++) {
    loopSensors();
    suml += leftColorData.g;
    sumr += rightColorData.g;
  }
  *left = suml / n;
  *right = sumr / n;
}
