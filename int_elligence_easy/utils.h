#ifndef UTILS_H
#define UTILS_H

// Helper Functions
int tcaselect(uint8_t i);
void lcount(void);
void rcount(void);

// Init Function
bool initRobot();

// Claw Functions
void ClawRaise();
void ClawLower();
void ClawClamp();
void ClawRelease();  
void clawInit();


// Sensor Function
void loopSensors();

// Motor Functions
void moveTo(int ltarget, int rtarget, double speed);
void forward(double dist, double speed);
void rotate(double angle, double speed);
void rotate(double angle, double speed);
void setMotorSpeeds(double left, double right);
void setRawMotorSpeeds(double left, double right);
void loopMotors();
void stopMotors();

// Pixy Cam Functions
void rotatePixyCamAndTOF(float deg);
void pixyInit();
#endif