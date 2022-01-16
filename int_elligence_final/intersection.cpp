#ifndef INTERSECTION_CPP
#define INTERSECTION_CPP
#include "global.h"
#include "utils.h"

char detectIntersection() {
    if(follower.readSensors() == S1_IN_S2_IN) {
        COLORTYPES leftC = leftColorData.color;
        COLORTYPES rightC = rightColorData.color;
        if (leftC == BLACK || rightC == BLACK)
        {
            leftBlack = (leftC == BLACK);
            rightBlack = (rightC == BLACK);
            return true;
        }
        else
        {
            leftGreen = (leftC == GREEN);
            rightGreen = (rightC == GREEN);
        }
        if (leftGreen || rightGreen)
        {
            Serial.println("Forward 2 cm");
            forward(2, 0.8);
            Serial.println("Done");
            return detectIntersection();
        }
    }
    return false;
}

void rotateUntilLeftColorBlack() {
  while(leftColorData.color != BLACK) {
    setMotorSpeeds(-0.5, 0.5);
    loopMotors();
    loopSensors();
  }
  rotate(-10, 0.5);
}
void rotateUntilRightColorBlack() {
  while(leftColorData.color != BLACK) {
    setMotorSpeeds(0.5, -0.5);
    loopMotors();
    loopSensors();
  }
  rotate(10, 0.5);
}

char doIntersection()
{
    Serial.println("Forward 10 cm");
    forward(10, 0.8);
    Serial.print(leftGreen);
    Serial.print('\t');
    Serial.print(rightGreen);
    Serial.print('\t');
    Serial.print(leftBlack);
    Serial.print('\t');
    Serial.print(rightBlack);
    Serial.print('\t');
    Serial.print(frontBlack);
    bool frontBlack = follower.readSensors() == S1_IN_S2_IN;
    if (leftGreen && rightGreen)
    {
        rotate(180, 0.8);
    }
    if (!leftGreen && !rightGreen)
    {
        if(!leftBlack && rightBlack && !frontBlack) 
        {
            rotateUntilRightColorBlack();
        }
        if (leftBlack && !rightBlack && !frontBlack)
        {
            rotateUntilLeftColorBlack();
        }
    }
    if (!leftGreen && rightGreen)
    {
        rotateUntilRightColorBlack();
    }
    if (leftGreen && !rightGreen)
    {
        rotateUntilLeftColorBlack();
    }
    return false;
}

#endif
