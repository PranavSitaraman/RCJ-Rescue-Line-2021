#ifndef EVAC_CPP
#define EVAC_CPP
#include "global.h"
#include "utils.h"
#include "evac.h"
void clawPickUp()
{
  ClawRelease();
  ClawLower();
  ClawClamp();
  ClawRaise();
}
void doSilver()
{
  if (cube_x > 165)
  {
    turn = 1;
  }
  else if (cube_x < 165)
  {
    turn = -1;
  }
  ClawRaise();
  float degreesTurned = Align2Object(turn);
  float distanceMoved = Move2Object();
  ClawRelease();
  ClawLower();
  ClawClamp();
  ClawRaise();
  forward(-distanceMoved, 85);
  rotate(-degreesTurned, 100);
  loopSensors();
  float TOF_holder = TOFdistance;
  while (TOFdistance > TOF_holder / 2)
  {
    loopSensors();
    setMotorSpeeds(0.35, 0.35);
    loopMotors();
  }
  stopMotors();
  switch (RescueZoneSearch())
  {
    case FRONT_CORNER:
    case RIGHT_CORNER:
      rotate(-45, 0.4);
    case LEFT_CORNER:
      rotate(-135, 0.4);
    case LEFT_CORNER*2:
      rotate(45, 0.4);
    case RIGHT_CORNER*2:
      rotate(135, 0.4);
  }
  while (TOFdistance > MAX_OBS_DIST)
  {
    loopSensors();
    setMotorSpeeds(0.35, 0.35);
    loopMotors();
  }
  ClawLower();
  ClawRelease();
  rotate(180, 0.5);
  int holding = 0;
  int holding_Two = 0;
  for (int x = 0; x <= 180; x++)
  {
    rotatePixyCamAndTOF(x);
    loopSensors();
    if (TOFdistance > holding)
    {
      holding = TOFdistance;
      holding_Two = x;
    }
    if (x == 180)
    {
      if (holding_Two > 90)
      {
        rotate(-(holding_Two - 90), 0.35);
      }
      else if (holding_Two < 90)
      {
        rotate((90 - holding_Two), 0.35);
      }
      rotatePixyCamAndTOF(90);
      while (TOFdistance > MAX_OBS_DIST)
      {
        loopSensors();
        setMotorSpeeds(0.35, 0.35);
        loopMotors();
      }
    }
  }
  forward(4, 0.4);
  setMotorSpeeds(-0.35, 0.35);
  loopSensors();
  while (rightColorData.color != GREEN && leftColorData.color != GREEN)
  {
    loopMotors();
    loopSensors();
  }
  stopMotors();
  setMotorSpeeds(-0.35, 0.35);
  loopSensors();
  while (rightColorData.color != BLACK || leftColorData.color != BLACK)
  {
    loopMotors();
    loopSensors();
  }
  stopMotors();
}
boolean FindSilver()
{
  uint8_t r_1, g_1, b_1;
  int silver_x = 0;
  int silver_y = 0;
  int silvercount = 0;
  int x_max = (pixy.frameWidth - 100);
  int y_max = (pixy.frameHeight - 15);
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int silver_x_arr[] = {0, 0};
  int silver_y_arr[] = {0, 0};

  loopMotors();


  Serial.print("    x_max   :");
  Serial.println(x_max);



  Serial.print("    y_max:");
  Serial.println(y_max);
  x = xoffset;

  while (x < x_max) {
    y = yoffset;
    silvercount = 0;
    while (y < y_max) //scan area can be adjusted by changing +/- to the edge of the frame
    {

      // Serial.print("scanning at x   :");
      //Serial.print(x);
      //Serial.print("   y   :");
      //Serial.println(y);
      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;


      // search for silver in 5x5 sqaure, starting at X and Y offset

      for (i = -2; i < 3; i++)
      {
        for (j = -2; j < 3; j++)
        {
          pixy.video.getRGB(x + i, y + j, &r_1, &g_1, &b_1, false);
          sum_r = sum_r + r_1;
          sum_g = sum_g + g_1;
          sum_b = sum_b + b_1;
        }
      }

      sum_r = sum_r / 25;
      sum_g = sum_g / 25;
      sum_b = sum_b / 25;

      if ((sum_r > 100) &&  (sum_g > 100) && ( sum_b < 220)) //  RGB for Silver
      { //a
        silvercount = silvercount + 1;

        silver_x = x;
        silver_y = y;

        if (silvercount > 3) {
          count = count + 1;
          if (count == 1)
          {
            silver_x_arr[0] = silver_x;
            silver_y_arr[0] = silver_y;
            silver_x_arr[1] = silver_x;
            silver_y_arr[1] = silver_y;
          }

          if (silver_x > silver_x_arr[1])
          {
            silver_x_arr[1] = silver_x;
          }

          if (silver_y > silver_y_arr[1])
          {
            silver_y_arr[1] = silver_y;
          }

          if (silver_y < silver_y_arr[0])
          {
            silver_y_arr[0] = silver_y;
            silver_x_arr[0] = silver_x;
          }

        }


      }//a

      y = y + 5;
    }
    x = x + 5;
  }
  if (silver_x_arr[0] > 0)
  {
    cube_x = (silver_x_arr[0] + silver_x_arr[1]) / 2;
    cube_y = (silver_y_arr[0] + silver_y_arr[1]) / 2;
    return (true);
  }
  return (false);
}
boolean FindBlack()
{
  uint8_t r_1, g_1, b_1;
  int black_x = 0;
  int black_y = 0;
  int blackcount = 0;
  int x_max = (pixy.frameWidth - 100);
  int y_max = (pixy.frameHeight - 15);
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int black_x_arr[] = {0, 0};
  int black_y_arr[] = {0, 0};
  loopMotors();
  x = xoffset;
  while (x < x_max)
  {
    y = yoffset;
    blackcount = 0;
    while (y < y_max)
    {
      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;
      for (i = -2; i < 3; i++)
      {
        for (j = -2; j < 3; j++)
        {
          pixy.video.getRGB(x + i, y + j, &r_1, &g_1, &b_1, false);
          sum_r = sum_r + r_1;
          sum_g = sum_g + g_1;
          sum_b = sum_b + b_1;
        }
      }
      sum_r = sum_r / 25;
      sum_g = sum_g / 25;
      sum_b = sum_b / 25;
      if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100))
      {
        blackcount = blackcount + 1;
        black_x = x;
        black_y = y;
        if (blackcount > 3)
        {
          count = count + 1;
          if (count == 1)
          {
            black_x_arr[0] = black_x;
            black_y_arr[0] = black_y;
            black_x_arr[1] = black_x;
            black_y_arr[1] = black_y;
          }
          if (black_x > black_x_arr[1])
          {
            black_x_arr[1] = black_x;
          }
          if (black_y > black_y_arr[1])
          {
            black_y_arr[1] = black_y;
          }
          if (black_y < black_y_arr[0])
          {
            black_y_arr[0] = black_y;
            black_x_arr[0] = black_x;
          }
        }
      }
      y = y + 5;
    }
    x = x + 5;
  }
  if (black_x_arr[0] > 0)
  {
    cube_x = (black_x_arr[0] + black_x_arr[1]) / 2;
    cube_y = (black_y_arr[0] + black_y_arr[1]) / 2;
    return (true);
  }
  return (false);
}
float Align2Object(int turn)
{
  float degree;
  switch (turn)
  {
    case -1:
      degree = (atan(fabs(165 - cube_x) / cube_y)) * 180 / PI;
      Serial.println(degree);
      rotate(degree, 100);
      break;
    case 0:
      break;
    case 1:
      degree = (atan(fabs(165 - cube_x) / cube_y)) * 180 / PI;
      rotate(-degree, 100);
      stopMotors();
      break;
  }
  return (degree);
}
float Move2Object()
{
  float true_y = 207 - cube_y;
  float equation_y = 0.005679282788 * (true_y) * (true_y) - 0.249685611 * (true_y) + 1.4695144 + 20;
  forward(equation_y, 85);
  stopMotors();
  return (equation_y);
}
int RescueZoneSearch()
{
  uint8_t r_1, g_1, b_1;
  int black_x = 0;
  int black_y = 0;
  int blackcount_L = 0;
  int blackcount_R = 0;
  int x_max = (pixy.frameWidth - 100);
  int y_max = (pixy.frameHeight - 15);
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int black_x_arr[] = {0, 0};
  int black_y_arr[] = {0, 0};
  rotatePixyCamAndTOF(0);
  loopSensors();
  if (TOFdistance > MAX_WALL_DIST)
  {
    x = 5;
    x_max = 315 / 2;
    int sum_r = 0;
    int sum_g = 0;
    int sum_b = 0;
    for (int z = 0; z != 2; z++)
    {
      y = 70;
      y_max = 130;
      if (z == 1)
      {
        x = x_max;
        x_max = 315;
      }
      while (y < y_max)
      {
        for (i = 0; i < (x_max - x);)
        {
          for (j = -2; j < 3;)
          {
            pixy.video.getRGB(x + i, y + j, &r_1, &g_1, &b_1, false);
            sum_r = sum_r + r_1;
            sum_g = sum_g + g_1;
            sum_b = sum_b + b_1;
          }
        }
        sum_r = (sum_r) / ((x_max - x) * (5));
        sum_g = (sum_g) / ((x_max - x) * (5));
        sum_b = (sum_b) / ((x_max - x) * (5));
        if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100))
        {
          if (z == 0)
          {
            blackcount_L = blackcount_L + 1;
          }
          else
          {
            blackcount_R = blackcount_R + 1;
          }
        }
        y = y + 5;
      }
    }
    if ((blackcount_R > 2) || (blackcount_L > 2))
    {
      if (blackcount_R > blackcount_L)
      {
        return (RIGHT_CORNER);
      }
      else
      {
        return (LEFT_CORNER);
      }
    }
    return (FRONT_CORNER);
  }
  else
  {
    rotatePixyCamAndTOF(180);
    x = 5;
    x_max = 315 / 2;
    int sum_r = 0;
    int sum_g = 0;
    int sum_b = 0;
    for (int z = 0; z != 2; z++)
    {
      y = 70;
      y_max = 130;
      if (z == 1)
      {
        x = x_max;
        x_max = 315;
      }
      while (y < y_max)
      {
        for (i = 0; i < (x_max - x);)
        {
          for (j = -2; j < 3;)
          {
            pixy.video.getRGB(x + i, y + j, &r_1, &g_1, &b_1, false);
            sum_r = sum_r + r_1;
            sum_g = sum_g + g_1;
            sum_b = sum_b + b_1;
          }
        }
        sum_r = (sum_r) / ((x_max - x) * (5));
        sum_g = (sum_g) / ((x_max - x) * (5));
        sum_b = (sum_b) / ((x_max - x) * (5));
        if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100))
        {
          if (z == 0)
          {
            blackcount_R = blackcount_R + 1;
          }
          else
          {
            blackcount_L = blackcount_L + 1;
          }
        }
        y = y + 5;
      }
    }
    if ((blackcount_R > 2) || (blackcount_L > 2))
    {
      if (blackcount_R > blackcount_L)
      {
        return (RIGHT_CORNER_R);
      }
      else
      {
        return (LEFT_CORNER_R);
      }
    }
    return (FRONT_CORNER);
  }
}
boolean detectEVAC()
{
  if (leftColorData.color == WHITE && leftColorData.color == WHITE)
  {
    if (rightColorData.g > WVALUE + 10 || leftColorData.g > WVALUE + 10)
    {
      return true;
    }
  }
  return false;
}
#endif
