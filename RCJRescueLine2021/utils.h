#ifndef UTILS_H
#define UTILS_H
int tcaselect(uint8_t i);
void lcount(void);
void rcount(void);
bool initRobot();
void ClawRaise();
void ClawLower();
void ClawClamp();
void ClawRelease();  
void clawInit();
void ClawVibration();
void loopSensors();
void moveTo(int ltarget, int rtarget, double speed);
void forward(double dist, double speed);
void rotate(double angle, double speed);
void rotate(double angle, double speed);
void setMotorSpeeds(double left, double right);
void setRawMotorSpeeds(double left, double right);
void loopMotors();
void stopMotors();
void rotatePixyCamAndTOF(float deg);
void pixyInit();
void calibration();
#endif
