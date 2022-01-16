  #ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>
#include <Pixy2.h>

// Robot Specifications
#define WHEEL_DIAMETER 4.45
#define ROBOT_LENGTH 12.5
#define ROBOT_WIDTH 14.5
#define ROBOT_DIAGONAL sqrt(ROBOT_LENGTH * ROBOT_LENGTH + ROBOT_WIDTH * ROBOT_WIDTH)
#define RG_RATIO 2
// Encoder Calculations
#define ENC_PER_ROT 360.0
#define ROT_ADJUST 1.1
#define ENC_PER_CM ENC_PER_ROT / (WHEEL_DIAMETER * PI) // Currently around 
#define ENC_PER_DEG ENC_PER_CM * ROBOT_DIAGONAL * PI / 360 * ROT_ADJUST

// Sensor Ports
#define LEFT_COLOR 5
#define RIGHT_COLOR 7
#define TOF 2
#define MIDWTHRESH 0.8

// Arm/Claw Pin
#define SERVO_PIN 44
#define BUZZER_PIN 45
#define SOUND_PIN A1
#define BATTERY_PIN A5

// Motor Deadzones to make sure movements run and arent stuck in the deadzone
#define MOTOR_DEADZONE (80.0/255.0)
#define KP 0.5
#define KI 0.005
#define KD 1.5
#define BASE_SPEED 0.3


// Obstacle Constants (ALL IN MM)
#define MAX_WALL_DIST 300//wall detection
#define MAX_OBS_DIST 120//initial detection
#define ROT_MAX_DIST 200//deteection druing rotation
#define TARGET_DIST ROBOT_WIDTH*10/2.0 + 200//circle follower
#define KP_OBS 4/1000

enum GyroAngle {X_ANGLE=1, Y_ANGLE, Z_ANGLE};

// Color Data Struct (Its the struct that holds all of the color data like RGB and the actual color)
struct ColorData {
  uint16_t r, g, b;
  COLORTYPES color;
};

// Get rid of extern errors in makeblock library
extern MePort_Sig mePort[17];
extern Encoder_port_type encoder_Port[6];

// Hardware
extern Servo ServoClaw;
extern Pixy2 pixy;
extern MeEncoderOnBoard leftm;
extern MeEncoderOnBoard rightm;
extern VL53L0X tof;
extern Adafruit_TCS34725 color;
extern MeLineFollower follower;
extern MeGyro gyro;
extern MeBuzzer buzzer;


// Line Following Variables
extern double prev_sum; // Previous Error Sum
extern double prev_error; // Previous Error

// Intersections
extern bool leftGreen;
extern bool rightGreen;
extern bool leftBlack;
extern bool rightBlack;
extern bool frontBlack;

// Calibration Variables
extern double GR_RATIO; // The green/red ratio for green
extern double WVALUE; // The normal white value
extern double BVALUE; // The normal black value

// Sensor Data
//.r:red .g:green .b:blue .color: identified color
extern ColorData rightColorData; // This holds the right color sensor data (using ColorData struct)
extern ColorData leftColorData; // This holds the left color sensor data (using ColorData struct)
extern uint16_t TOFdistance; // TOF distance


// Evac Data
extern int xoffset;
extern int yoffset;
extern int cube_x;
extern int cube_y;
extern int angle_cube;
extern int turn;
extern int rescueSide;

#define FRONT_CORNER 0 
#define LEFT_CORNER 1 
#define LEFT_CORNER_R 2 
#define RIGHT_CORNER 3 
#define RIGHT_CORNER_R 4 

// Obstacle Data
extern bool tracingDir;

#endif
