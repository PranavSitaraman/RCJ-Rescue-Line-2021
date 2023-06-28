#ifndef EVAC_CPP
#define EVAC_CPP
#include "global.h"
#include "utils.h"
#include "evac.h"
void FirstNoSilverMvmt()
{
  forward(45, MOTOR_SPEED);
  rotatePixyCamAndTOF(0);
  loopSensors();
  if (TOFdistance > MAX_WALL_DIST) rotate(-90, MOTOR_SPEED);
  else rotate(90, MOTOR_SPEED);
  rotatePixyCamAndTOF(90);
}
void DepositVictim()
{
  forward(-5, MOTOR_SPEED);
  rotate(140, MOTOR_SPEED);
  ClawRelease();
  ClawVibration();
  ClawRaise();
}
void clawPickUp()
{
  ClawRelease();
  ClawLower();
  ClawClamp();
  ClawRaise();
}
boolean FindSilver()
{
  uint8_t r_1, g_1, b_1;
  int silvercount = 0;
  int x_max = 300;
  int y_max = 200;
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int silver_x_arr[] = {0, 0};
  int silver_y_arr[] = {0, 0};
  int xoffset, yoffset;
  int save[] = {0, 0, 0};
  cube_x = 0;
  cube_y = 0;
  maxsilvercount = 0;
  xoffset = 10; yoffset = 100;
  x = xoffset;
  while (x < x_max)//
  {
    y = yoffset;
    silvercount = 0;
    while (y < y_max)
    {
      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;
      for (i = -5; i < 5; i++)
      {
        for (j = -5; j < 5; j++)
        {
          pixy.video.getRGB(x + i, y + j, & r_1,  & g_1,  & b_1, false);
          if (r_1 <= 100 && g_1 <= 100 && b_1 <= 100 )
          {
            sum_r = sum_r + 50;
            sum_g = sum_g + 50;
            sum_b = sum_b + 50;
          }
          if ((r_1 < 225 && r_1 > 100) && (g_1 < 225 && g_1 > 100) && (b_1 < 225 && b_1 > 100))
          {
            sum_r = sum_r + 150;
            sum_g = sum_g + 150;
            sum_b = sum_b + 150;
          }
          if ((r_1 >= 225) && (g_1 >= 225) && (b_1 >= 225))
          {
            sum_r = sum_r + 200;
            sum_g = sum_g + 200;
            sum_b = sum_b + 200;
          }
        }
      }
      sum_r = sum_r / 100;
      sum_g = sum_g / 100;
      sum_b = sum_b / 100;
      if ((sum_r > 150 && sum_r < 200) && (sum_g > 150 && sum_g < 200) && (sum_b > 150 && sum_b < 200))
      {
        silvercount = silvercount + 1;
        if (silvercount >= 3)
        {
          count = count + 1;
          if (count == 1)
          {
            silver_x_arr[0] = x;
            silver_y_arr[0] = y + j - 10 * silvercount;
          }
          int avgPos = 0;
          if (count != 1 && avgPos != 3)
          {
            save[avgPos] = x;
            if ((save[avgPos] + silver_x_arr[0]) / 2 > 50) silver_x_arr[0] = save[avgPos];
            avgPos++;
          }
          if (x + i > silver_x_arr[0])
          {
            silver_x_arr[1] = silver_x_arr[0] + 20;
            silver_y_arr[1] = silver_y_arr[0] + 20;
          }
        }
      }
      y = y + 10;
    }
    x = x + 10;
  }
  if (silver_x_arr[0] > 0)
  {
    cube_x = (silver_x_arr[0] + silver_x_arr[1]) / 2;
    cube_y = silver_y_arr[0];
    return (true);
  }
  return (false);
}
int FindSilverDirection()
{
  if (FindSilver())
  {
    if (cube_x > 158) return (1);
    else if (cube_x < 158) return (-1);
    return (0);
  }
  else return (NO_SILVER);
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
          if (silver_x > silver_x_arr[1]) silver_x_arr[1] = silver_x;
          if (silver_y > silver_y_arr[1]) silver_y_arr[1] = silver_y;
          if (silver_y < silver_y_arr[0]) silver_y_arr[0] = silver_y;
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
int FindBlackDirection()
{
  if (FindBlack())
  {
    if (cube_x > 158) return (1);
    else if (cube_x < 158) return (-1);
    return (0);
  }
  else return (NO_BLACK);
}
float Align2Object(int turn)
{
  float degree;
  switch (turn)
  {
    case -1:
      degree = (atan(fabs(158 - cube_x) / ((207 - cube_y) + (37 * (12 / 5))) ) ) * 180 / PI;
      if (degree > 23) degree = degree - 23;
      else degree = degree - 5;
      rotate(degree, MOTOR_SPEED);
      break;
    case 0:
      break;
    case 1:
      degree = (atan(fabs(158 - cube_x) / ((207 - cube_y) + (37 * (12 / 5))) ) ) * 180 / PI ;
      if (degree > 23) degree = degree - 23;
      rotate(-degree, MOTOR_SPEED);
      break;
    case NO_SILVER:
      break;
    case NO_BLACK:
      break;
  }
  stopMotors();
  return (degree);
}
float Move2Object()
{
  float true_y = 207 - cube_y;
  float equation_y = 0.005679282788 * (true_y) * (true_y) - 0.249685611 * (true_y) + 1.4695144 + 37;
  forward(equation_y, MOTOR_SPEED);
  stopMotors();
  return (equation_y);
}
int RescueZoneSearch()
{
  uint8_t r_1, g_1, b_1;
  int blackcount_L = 0;
  int blackcount_R = 0;
  int x_max = (pixy.frameWidth - 100);
  int y_max = (pixy.frameHeight - 15);
  int i, j = 0;
  int x, y = 0;
  rotatePixyCamAndTOF(0);
  loopSensors();//check TOF
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
          if (z == 0) blackcount_L = blackcount_L + 1;
          else blackcount_R = blackcount_R + 1;
        }
        y = y + 5;
      }
    }
    if ((blackcount_R > 2) || (blackcount_L > 2))
    {
      if (blackcount_R > blackcount_L) return (RIGHT_CORNER);
      else return (LEFT_CORNER);
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
          if (z == 0) blackcount_L = blackcount_L + 1;
          else blackcount_R = blackcount_R + 1;
        }
        y = y + 5;
      }
    }
    if ((blackcount_R > 2) || (blackcount_L > 2))
    {
      if (blackcount_R > blackcount_L) return (RIGHT_CORNER_R);
      else return (LEFT_CORNER_R);
    }
    return (FRONT_CORNER);
  }
}
void compactRescueSearch(bool InitPosition)
{
  if (!(InitPosition)) forward(45, MOTOR_SPEED);
  stopMotors();
  switch (RescueZoneSearch())
  {
    case FRONT_CORNER:
      break;
    case RIGHT_CORNER:
      rotate(-70, MOTOR_SPEED);
      break;
    case LEFT_CORNER:
      rotate(-110, MOTOR_SPEED);
      break;
    case LEFT_CORNER_R:
      rotate(70, MOTOR_SPEED);
      break;
    case RIGHT_CORNER_R:
      rotate(110, MOTOR_SPEED);
      break;
  }
  stopMotors();
  rotatePixyCamAndTOF(90);
  loopSensors();
  while (TOFdistance > 250)
  {
    loopSensors();
    setMotorSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    loopMotors();
  }
  forward(5, MOTOR_SPEED);
  stopMotors();
}
boolean detectEVAC()
{
  loopMotors();
  loopSensors();
  if (leftColorData.temp < 5750 && rightColorData.temp < 5500)
  {
    if (leftColorData.color != BLACK && rightColorData.color != BLACK)
    {
      return true;
    }
  }
  return false;
}
void doEvac()
{
  EVAC_init();
  turn = FindSilverDirection();
  bool flag = false;
  if (turn == NO_SILVER)
  {
    FirstNoSilverMvmt();
    flag = true;
    turn = FindSilverDirection();
  }
  float degreesTurned = Align2Object(turn);
  VictimRescue(degreesTurned);
  compactRescueSearch(flag);
  DepositVictim();
  rotate(-50, MOTOR_SPEED);
  turn = Findgreen();
  degreesTurned = Align2Object(turn);
  loopSensors();
  while (leftColorData.color != GREEN)
  {
    loopMotors();
    setMotorSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    loopSensors();
  }
  forward(7, MOTOR_SPEED);
  loopSensors();
}
void VictimRescue(float degreesTurned)
{
  float distanceMoved = Move2Object();
  stopMotors();
  ClawLower();
  ClawRaise();
  forward(-distanceMoved, MOTOR_SPEED);
  rotate(-degreesTurned, MOTOR_SPEED);
}
void EVAC_init()
{
  stopMotors();
  setMotorSpeeds(0, 0);
  loopSensors();
  ClawRaise();
  stopMotors();
}
void SearchLoop()
{
  for (bool i = false; i != true;)
  {
    float degreesTurned = Align2Object(turn);
    if (turn == NO_SILVER)
    {
      turn = FindBlackDirection();
      if (turn == NO_BLACK) i = true;
      else VictimRescue(degreesTurned);
    }
    else VictimRescue(degreesTurned);
  }
}
bool Findgreen()
{
  uint8_t r_1, g_1, b_1;
  int greencount = 0;
  int x_max = 300;
  int y_max = 200;
  int i, j = 0;
  int x, y = 0;
  int count = 0;
  int green_x_arr[] = {0, 0};
  int green_y_arr[] = {0, 0};
  int xoffset, yoffset;
  int save[] = {0, 0, 0};
  cube_x = 0;
  cube_y = 0;
  maxgreencount = 0;
  xoffset = 10; yoffset = 100;
  pixy.video.getRGB(185, 145, & r_1,  & g_1,  & b_1, false);
  x = xoffset;
  while (x < x_max)
  {
    y = yoffset;
    greencount = 0;
    while (y < y_max)
    {
      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;
      for (i = -5; i < 5; i++)
      {
        for (j = -5; j < 5; j++)
        {
          pixy.video.getRGB(x + i, y + j, & r_1,  & g_1,  & b_1, false);
          sum_r = sum_r + r_1;
          sum_g = sum_g + g_1;
        }
      }
      sum_r = sum_r / 100;
      sum_g = sum_g / 100;
      sum_b = sum_b / 100;
      if (sum_g / sum_r > 1.4)
      {
        greencount = greencount + 1;
        if (greencount >= 1)
        {
          count = count + 1;
          if (count == 1)
          {
            green_x_arr[0] = x;
            green_y_arr[0] = y + j - 10 * greencount;
          }
          if (x + i > green_x_arr[0])
          {
            green_x_arr[1] = green_x_arr[0] + 20;
            green_y_arr[1] = green_y_arr[0] + 20;
          }
        }
      }
      y = y + 10;
    }
    x = x + 10;
  }
  if (green_x_arr[0] > 0)
  {
    cube_x = (green_x_arr[0] + green_x_arr[1]) / 2;
    cube_y = (green_y_arr[0] + green_y_arr[1]) / 2;
    return (true);
  }
  return (false);
}
#endif
