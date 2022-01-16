#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define rightMotor 0
#define leftMotor 1
extern MePort_Sig mePort[17];
extern Encoder_port_type encoder_Port[6];
extern MeEncoderOnBoard LMotor;
extern MeLineFollower lineFinder;
extern MeEncoderOnBoard RMotor;
extern MeUltrasonicSensor ultrasonic_10;
extern float obsr;
extern float threshold;
extern float g;
extern float h;
extern float buff;
extern long Enc[2];
extern long Pulse[2];
extern float forwardsensor;
extern float wheelbase;
extern float wheeldiameter;
extern float sensoraverage;
extern float Kp;
extern float Ki;
extern float Kd;
extern float leftvalue;
extern float obstacleturn;
extern float smallobstacleturn;
extern float rightvalue;
extern float integraluplimit;
extern float tar;
extern float inner;
extern float outer;
extern float error;
extern float turn;
extern float powerB;
extern float powerC;
extern float integraldownlimit;
extern float leftrightratio;
extern float integral;
extern float integraltarget;
extern float derivative;
extern float obstaclemotor;
extern float reverseobstaclemotor;
extern float motortarget;
extern float reversemotortarget;
extern float derivativesetpoint;
extern float lastError;
extern float xvalue;
extern float avalue;
extern float prevx;
extern float preva;
#endif
