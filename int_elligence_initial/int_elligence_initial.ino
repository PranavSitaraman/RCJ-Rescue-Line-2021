#include "global.h"
#include "utils.h"
#include "obstacle.h"
#include "linetracing.h"
MeEncoderOnBoard LMotor(SLOT2);
MeLineFollower lineFinder(PORT_9);
MeEncoderOnBoard RMotor(SLOT1);
MeUltrasonicSensor ultrasonic_10(10);
MePort_Sig mePort[17] =
{
 { NC, NC }, {   5,   4 }, {   3,   2 }, {   7,   6 }, {   9,   8 }, 
 { 16, 17 }, { A10, A15 }, {  A9, A14 }, {  A8, A13 }, {  A7, A12 }, 
 //             LIGHT2        LIGHT1        TEMP          SOUND
 { A6,A11 }, {  NC,  A2 }, {  NC,  A3 }, {  NC,  A0 }, {  NC,  A1 },
 { NC, NC }, { NC, NC },
};
Encoder_port_type encoder_Port[6] =
{
  { NC,     NC,     NC,     NC,     NC},
  //ENA A   ENA B   PWMA    DIR A2  DIR A1
  { 19,     42,     11,     49,     48},
  //ENB A   ENB B   PWMB    DIR B1  DIR B2
  { 18,     43,     10,     47,     46},
  { NC,     NC,     NC,     NC,     NC},
  { NC,     NC,     NC,     NC,     NC},
};
float obsr = 3;
float threshold = 35;
float g = 1;
float h = 150;
float buff = 3;
long Enc[2] = {0};
long Pulse[2] = {0};
float forwardsensor = 10;
float wheelbase = 20;
float wheeldiameter = 4;
float sensoraverage = 50;
float Kp = 5.75;
float Ki = 0.01;
float Kd = 0.1;
float leftvalue = 100;
float obstacleturn = 70;
float smallobstacleturn = 25;
float rightvalue = 100;
float integraluplimit = 50;
float tar = leftvalue - rightvalue;
float inner = ((obsr + buff + forwardsensor)/(obsr + buff + forwardsensor + wheelbase)) * h;
float outer = h;
float error;
float turn;
float powerB;
float powerC;
float integraldownlimit = -integraluplimit;
float leftrightratio = 1.7;
float integral = 0;
float integraltarget = 0;
float derivative = 0;
float obstaclemotor = 100;
float reverseobstaclemotor = -obstaclemotor;
float motortarget = 100;
float reversemotortarget = -motortarget;
float derivativesetpoint = 0;
float lastError = 0;
float xvalue = 0;
float avalue = 0;
float prevx = 0;
float preva = 0;
void setup()
{
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(LMotor.getIntNum(), isr_LMotorEncoder, RISING);
  attachInterrupt(RMotor.getIntNum(), isr_RMotorEncoder, RISING);
  resetmotors();
}
void loop() 
{
  RMotor.loop();
  LMotor.loop();
  //obstacleavoidance();
  linetracing();
}
