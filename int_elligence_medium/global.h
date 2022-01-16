#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>
#include <Pixy2.h>
#define WHEEL_DIAMETER 4.45
#define ROBOT_LENGTH 12.5
#define ROBOT_WIDTH 14.5
#define ROBOT_DIAGONAL sqrt(ROBOT_LENGTH * ROBOT_LENGTH + ROBOT_WIDTH * ROBOT_WIDTH)
#define RG_RATIO 2
#define ENC_PER_ROT 39.267 * 9
#define ROT_ADJUST 1.1
#define ENC_PER_CM ENC_PER_ROT / (WHEEL_DIAMETER * PI) // Currently around 
#define ENC_PER_DEG ENC_PER_CM * ROBOT_DIAGONAL * PI / 360 * ROT_ADJUST
#define LEFT_COLOR 5
#define RIGHT_COLOR 7
#define TOF 2
#define TOF_GROUND 3
#define MIDWTHRESH 0.8
#define SERVO_PIN 44
#define BUZZER_PIN 45
#define SOUND_PIN A1
#define BATTERY_PIN A5
#define MOTOR_DEADZONE 0.15
#define KP 0.9
#define KI 0.0
#define KD 0.0
#define BASE_SPEED 0.25
#define SKEW_RIGHT 1.26
#define MAX_WALL_DIST 600
#define MAX_OBS_DIST 120
#define ROT_MAX_DIST 300
#define TARGET_DIST ROBOT_WIDTH*10/2.0 + 200
#define KP_OBS 4/1000
enum GyroAngle {X_ANGLE = 1, Y_ANGLE, Z_ANGLE};
struct ColorData
{
  uint16_t r, g, b, temp;
  COLORTYPES color;
};
extern MePort_Sig mePort[17];
extern Encoder_port_type encoder_Port[6];
extern Servo ServoClaw;
extern Pixy2 pixy;
extern VL53L0X tof;
extern Adafruit_TCS34725 color;
extern MeGyro gyro;
extern double prev_sum;
extern double prev_error;
extern bool leftGreen;
extern bool rightGreen;
extern bool leftBlack;
extern bool rightBlack;
extern bool frontBlack;
extern double GR_RATIO;
extern double WVALUE;
extern double BVALUE;
extern ColorData rightColorData;
extern ColorData leftColorData;
extern uint16_t TOFdistance;
extern int xoffset;
extern int yoffset;
extern int cube_x;
extern int cube_y;
extern int angle_cube;
extern int turn;
extern int rescueSide;
extern int maxsilvercount;
extern int silverCountTape;
#define FRONT_CORNER 0
#define LEFT_CORNER 1
#define LEFT_CORNER_R 2
#define RIGHT_CORNER 3
#define RIGHT_CORNER_R 4
extern bool tracingDir;
#define lMotorPwm 11
#define lMotor1 48
#define lMotor2 49
#define lInterrupt 19
#define lChannelB 43
#define rMotorPwm 10
#define rMotor1 47
#define rMotor2 46
#define rInterrupt 18
#define rChannelB 42
#define MAX_ACC 20.0
extern double lTarPwm;
extern double lCurPwm;
extern double rTarPwm;
extern double rCurPwm;
extern int lEnc;
extern int rEnc;
extern long prevLoopMillis;
#endif
