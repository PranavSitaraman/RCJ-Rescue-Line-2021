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
void doEvac()
{
  setRawMotorSpeeds(0, 0);
  loopSensors();
  ClawRaise();
  if (FindSilver())
  {
    if (cube_x > 158)
    {
      turn = 1;
    }
    else if (cube_x < 158)
    {
      turn = -1;
    }
    float degreesTurned = Align2Object(turn);
    stopMotors();
    float distanceMoved = Move2Object();
    stopMotors();
    forward(2, 0.35);
    stopMotors();
    ClawRaise();
    rotate(10, 0.25);
    ClawLower();
    ClawRaise();
    rotate(-12, 0.35);
    stopMotors();
    forward(-distanceMoved + 10, 0.35);
    stopMotors();
    rotate(-degreesTurned, 0.35);
    stopMotors();
  }
  loopSensors();
  float TOF_holder = TOFdistance;
  forward(45, 0.35);
  stopMotors();
  switch (RescueZoneSearch())
  {
    case FRONT_CORNER:
      break;
    case RIGHT_CORNER:
      rotate(-70, 0.4);
      break;
    case LEFT_CORNER:
      rotate(-110, 0.4);
      break;
    case LEFT_CORNER_R:
      rotate(70, 0.4);
      break;
    case RIGHT_CORNER_R:
      rotate(110, 0.4);
      break;
  }
  stopMotors();
  rotatePixyCamAndTOF(90);
  loopSensors();
  while (TOFdistance > 330)
  {
    loopSensors();
    setRawMotorSpeeds(0.35, 0.35);
    loopMotors();
  }
  stopMotors();
  forward(7, 0.35);
  stopMotors();
  ClawVibration();
  rotate(80, 0.35);
  loopSensors();
  while (leftColorData.color != GREEN && rightColorData.color != GREEN)
  {
    loopSensors();
    setRawMotorSpeeds(0.35, 0.35);
    loopMotors();
  }
  stopMotors();
  forward(5, 0.35);
  stopMotors();
  ClawRaise();
}
boolean FindSilver()
{
  uint8_t r_1, g_1, b_1;
  int silver_x = 0;
  int silver_y = 0;
  int silvercount = 0;
  int x_max = 300;
  int y_max = 200;
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int silver_x_arr[] = {0, 0};
  int silver_y_arr[] = {0, 0};
  int xoffset, yoffset;
  cube_x = 0;
  cube_y = 0;
  maxsilvercount = 0;
  xoffset = 10; yoffset = 100;
  x = xoffset;
  while (x < x_max)
  {
    int lastsilvercount = 0;
    y = yoffset;
    silvercount = 0;
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
      if ((sum_r > 130 && sum_r < 230) &&  (sum_g > 130 && sum_g < 230) && ( sum_b > 130 && sum_b < 230))
      {
        silvercount = silvercount + 1;
        silver_x = x + i;
        silver_y = y + j;
        if (silvercount > 3)
        {
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
          if ((silver_x == silver_x_arr[0]) && (silvercount > lastsilvercount))
          {
            silver_y_arr[0] = silver_y;
          }
          if (silvercount > maxsilvercount)
          {
            silver_x_arr[1] = silver_x;
            maxsilvercount = silvercount;
          }
        }
      }
      lastsilvercount = silvercount;
      y = y + 5;
    }
    x = x + 5;
  }
  if (silver_x_arr[0] > 0)
  {
    cube_x = (silver_x_arr[0] + silver_x_arr[1]) / 2;
    cube_y = silver_y_arr[0];
    return (true);
  }
  return (false);
}
boolean FindBlack()
{
  uint8_t r_1, g_1, b_1;
  int silver_x = 0;
  int silver_y = 0;
  int silvercount = 0;
  int x_max = 300;
  int y_max = 200;
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int silver_x_arr[] = {0, 0};
  int silver_y_arr[] = {0, 0};
  int xoffset, yoffset;
  cube_x = 0;
  cube_y = 0;
  xoffset = 10; yoffset = 100;
  x = xoffset;
  while (x < x_max)
  {
    y = yoffset;
    silvercount = 0;
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
      if ((sum_r < 100) &&  (sum_g < 100) && (sum_b < 100))
      {
        silvercount = silvercount + 1;
        silver_x = x + i;
        silver_y = y + j;
        if (silvercount > 3)
        {
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
          }
        }
      }
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
float Align2Object(int turn)
{
  float degree;
  switch (turn)
  {
    case -1:
      degree = (atan(fabs(158 - cube_x) / ((207 - cube_y) + (37 * (12 / 5))) ) ) * 180 / PI;
      if (degree > 23)
      {
        degree = degree - 23;
      }
      else
      {
        degree = degree - 5;
      }
      rotate(degree, 0.35);
      break;
    case 0:
      break;
    case 1:
      degree = (atan(fabs(158 - cube_x) / ((207 - cube_y) + (37 * (12 / 5))) ) ) * 180 / PI;
      if (degree > 23)
      {
        degree = degree - 23;
      }
      rotate(-degree, 0.35);
      stopMotors();
      break;
  }
  return (degree);
}
float Move2Object()
{
  float true_y = 207 - cube_y;
  float equation_y = 0.005679282788 * (true_y) * (true_y) - 0.249685611 * (true_y) + 1.4695144 + 37;
  forward(equation_y, 0.35);
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
      y = 100;
      y_max = 150;
      if (z == 1)
      {
        x = 315 / 2;
        x_max = 315;
      }
      while (y < y_max)
      {
        for (i = 0; i < (x_max - x); i++)
        {
          for (j = -2; j < 3; j++)
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
      y = 100;
      y_max = 150;
      if (z == 1)
      {
        x = 315 / 2;
        x_max = 315;
      }
      while (y < y_max)
      {
        for (i = 0; i < (x_max - x); i++)
        {
          for (j = -2; j < 3; j++)
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
  if ((leftColorData.temp < 5500 || rightColorData.temp < 5500))
  {
    return true;
  }
  return false;
}
#endif
