#ifndef UTILS_H
#define UTILS_H
int tcaselect(uint8_t);
void lcount();
void rcount();
void initRobot();
void ClawRaise();
void ClawLower();
void ClawClamp();
void ClawRelease();
void clawInit();
void ClawVibration();
void loopSensors();
void moveTo(int, int, double);
void forward(double, double);
void rotate(double, double);
void rotate(double, double);
void setRawMotorSpeeds(double, double);
void loopMotors();
void stopMotors();
void rotatePixyCamAndTOF(float);
void pixyInit();
#endif
