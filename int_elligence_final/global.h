#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>
#include <Pixy2.h>
#include <math.h>

#define WHEEL_DIAMETER 4.45 /*THe Diameter of the Wheel+base of treads; it does not include the bumps*/
#define ROBOT_DIAGONAL 20 /*diagonal distance of contact zone from wheel to wheel.*/
#define ROBOT_WIDTH 14.5 /*Width of bot*/
#define ENC_PER_ROT 360.0/*in one rotation, there are 360 encoders recorded*/
#define ROT_ADJUST 1.1 /*Small adjust for rotation*/
#define ENC_PER_CM /*=*/ ENC_PER_ROT / (WHEEL_DIAMETER * PI)//Encoders per centimeter
#define ENC_PER_DEG /*=*/ ENC_PER_CM * ROBOT_DIAGONAL * PI / 360 * ROT_ADJUST //Encoders per 1 degree of a turn
#define LEFT_COLOR 5 //Left Color Mux channel
#define RIGHT_COLOR 7 //Right Color Mux channel
#define TOF 4 //Time of Flight Mux channel
#define MOTOR_DEADZONE (80.0/255.0) //Deadzone of the motore where it cannot move
#define KP 1.6 //Proportional constant
#define KI 0.04 //Integral constant
#define KD 1.3 //Derivative constant
#define BASE_SPEED 0.3 //Base speed of line tracing
#define PIXY_OFFSET 50 //Offset of PixyCam from center of rotation of the bot; used in calculating turns using x+y coordinates for align2object
//Color Variables/types
struct ColorData {
  uint16_t r, g, b;
  COLORTYPES color;
};

// Hardware
extern int servoPin;
extern Servo ServoClaw;
extern Pixy2 pixy;
extern MePort_Sig mePort[17];
extern Encoder_port_type encoder_Port[6];
extern MeEncoderOnBoard leftm;
extern MeEncoderOnBoard rightm;
extern VL53L0X tof;
extern Adafruit_TCS34725 color;
extern MeLineFollower follower;

// Line Following
extern double prev_sum;
extern double prev_error;

// Intersections
extern bool leftGreen;
extern bool rightGreen;
extern bool leftBlack;
extern bool rightBlack;
extern bool frontBlack;

// Calibration Variables
extern double GR_RATIO;
extern double WVALUE;
extern double BVALUE;

// Sensor Data
extern ColorData rightColorData;
extern ColorData leftColorData;
extern uint16_t TOFdistance;

//Evac Data
extern int xoffset;
extern int yoffset;
extern int cube_x;
extern int cube_y;
extern int angle_cube;
extern int turn;
#endif
