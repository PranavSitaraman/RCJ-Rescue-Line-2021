#ifndef UTILS_CPP
#define UTILS_CPP
#include "global.h"
void clawInit()
{
  ServoClaw.write(95);
  delay(1000);
}
void pixyInit()
{
  pixy.setServos(90,450);
  delay(1000);
}

void clawRaise()
{//Raises Claw
  ServoClaw.write(90); 
  delay(1000);
}
void clawLower()
{//Lowers Claw
  for(int x = 90; x>=30;){
  ServoClaw.write(x); 
  x=x-5;
  delay(20*5);
  }
}
void clawRelease()
{//Releases object in claw
  
  for(int x=0;x<=720;x++){
  pixy.setServos(x,450);
  delay(1);
  }
}
void clawClamp()
{//Clamps the claw on an object
  for(int x=720;x>=0;x--){
  pixy.setServos(x,450);
  delay(1);
  }
}
void isr_l(void)
{
    if (digitalRead(leftm.getPortB()) == 0)
    {
        leftm.pulsePosMinus();
    }
    else
    {
        leftm.pulsePosPlus();
    }
}
void isr_r(void)
{
    if (digitalRead(rightm.getPortB()) == 0)
    {
        rightm.pulsePosMinus();
    }
    else
    {
        rightm.pulsePosPlus();
    }
}
int tcaselect(uint8_t i)
{
    if (i > 7)
        return -4;
    Wire.beginTransmission(0x70);
    Wire.write(1 << i);
    return (Wire.endTransmission());
}
bool initRobot()
{
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
    attachInterrupt(leftm.getIntNum(), isr_l, RISING);
    attachInterrupt(rightm.getIntNum(), isr_r, RISING);
    tcaselect(RIGHT_COLOR);
    if (!color.begin())
    {
        return false;
    }
    tcaselect(LEFT_COLOR);
    if (!color.begin())
    {
        return false;
    }
    tcaselect(TOF);
    if (!tof.init())
    {
        return false;
    }
    tof.startContinuous();
    ServoClaw.attach(servoPin); 
    return true;
}
void setMotorSpeeds(double left, double right, bool rawSpeed=false)
{
    // Make sure you the speeds arent higher then 1
    if(fabs(left) >= 1) 
    {
        left = copysign(1, left);  
    }
    if(fabs(right) >= 1) 
    {
        right = copysign(1, right);  
    }
    // Checks if the left speed that we are setting is less then deadzone
    if (fabs(left) < MOTOR_DEADZONE)
    {
        // Checks if the left speed is much less then the deadzone
        if(fabs(left) < MOTOR_DEADZONE / 5) 
        {
            // Since it is, we set the speed to 0
            left = 0;
        } else {
            // If it isnt, we set it to +/- the deadzone
            left = copysign(MOTOR_DEADZONE, left);
        }
    }
    // Checks if the right speed that we are setting is less then deadzone
    if (fabs(right) < MOTOR_DEADZONE)
    {
        // Checks if the right speed is much less then the deadzone
        if (fabs(right) < MOTOR_DEADZONE / 5)
        {
            // Since it is, we set the speed to 0
            right = 0;
        } else {
            // If it isnt, we set it to +/- the deadzone
            right = copysign(MOTOR_DEADZONE, right);
        }
    }

    if(rawSpeed){
        // Set the speeds of the motors
        leftm.setMotorPwm(-left * 255);
        rightm.setMotorPwm(right * 255);
    }
    else
    {
        // Set the speeds of the motors
        leftm.setTarPWM(-left * 255);
        rightm.setTarPWM(right * 255);
    }
}
void loopMotors()
{
    // Loop the motors so speeds and encoders update
    leftm.loop();
    rightm.loop();
}
void stopMotors()
{
    // This fully stops the motor because set Tar PWM sets the target pwm,
    // this directly sets the motor pwm without PID being added with encoders
    leftm.setMotorPwm(0);
    rightm.setMotorPwm(0);
}
void moveTo(int ltarget, int rtarget, double speed)
{
    // Reset Encoders
    leftm.setPulsePos(0);
    rightm.setPulsePos(0);
    
    // Set motor speeds (copysigns are so they go in the right direction)
    setMotorSpeeds(copysign(speed, ltarget), copysign(speed, rtarget));

    // Update motors until we reach target pos
    while (abs(leftm.getCurPos()) < abs(ltarget) || abs(rightm.getCurPos()) < abs(rtarget))
    {
        loopMotors();
    }
    stopMotors();
}
/**
 * Simple Wrapper to moveTo with ENC_PER_CM calculations, so you can go forward in centimeters
 */
void forward(double dist, double speed)
{
    moveTo(dist * ENC_PER_CM, dist * ENC_PER_CM, speed);
}
/**
 * Simple Wrapper to moveTo with ENC_PER_DEG calculations, so you can go rotate in degrees
 */
void rotate(double angle, double speed)
{
    moveTo(angle * ENC_PER_DEG, -angle * ENC_PER_DEG, speed);
}

void loopSensors()
{
    uint16_t c;
    tcaselect(RIGHT_COLOR);
    color.getRawData(&rightColorData.r, &rightColorData.g, &rightColorData.b, &c);
    Serial.println(rightColorData.g);
    if (rightColorData.g < BVALUE * 1.5)
    {
        rightColorData.color = BLACK;
    }
    else if ((double) rightColorData.g / (double) rightColorData.r > GR_RATIO)
    {
        rightColorData.color = GREEN;
    }
    else
    {
        rightColorData.color = WHITE;
    }
    tcaselect(LEFT_COLOR);
    color.getRawData(&leftColorData.r, &leftColorData.g, &leftColorData.b, &c);
    Serial.println(leftColorData.g);
    if (leftColorData.g < BVALUE * 1.5)
    {
        leftColorData.color = BLACK;
    }
    else if ((double) leftColorData.g / (double) leftColorData.r > GR_RATIO)
    {
        leftColorData.color = GREEN;
    }
    else
    {
        leftColorData.color = WHITE;
    }
    /*
    TOFdistance = tof.readRangeContinuousMillimeters();
    Serial.println(TOFdistance);*/
}

void rotatePixyCamAndTOF(float deg) 
{
  pixy.setServos(0, deg);  
}
#endif
