#ifndef EVAC_CPP
#define EVAC_CPP
#include "global.h"
#include "utils.h"

void clawPickUp()
{
  Serial.println("Claw Release");
  ClawRelease();
  ClawLower();
  Serial.println("Claw Clamp");
  ClawClamp();
  Serial.println("Claw Raise");
  ClawRaise();
}

boolean FindSilver()
{ //find silver funciton start
  // scan 5x5 area starting from X and Y offset (in this case selected as x=5, y=100
  // after completing all sqaure of 5x5 pixels along the y-axis, offset is shifted to scan the next set of 5x5 squares along the height of the frame
  // 5 continuous silverdots  is taken as cofirmed silver object.
  uint8_t r_1, g_1, b_1;//color values used for identifying color values of a pixel in the video outuput of the pixy
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

  rightm.loop();
  leftm.loop();


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

    } //while loop to complete scan in y


    x = x + 5;

  } //end of scan for x-axis
  if (silver_x_arr[0] > 0)
  {
    cube_x = (silver_x_arr[0] + silver_x_arr[1]) / 2;
    cube_y = (silver_y_arr[0] + silver_y_arr[1]) / 2;
    Serial.print("Sliver center (X,Y):"); Serial.print("\t");
    Serial.print(cube_x); Serial.print("\t Y :"); Serial.print("\t"); Serial.println(cube_y);
    return (true);
  }
  return (false);
}
//SILVER SEARCH END==============================================================
//===============================================================================
//===============================================================================
//===============================================================================
//===============================================================================
//BLACK SEARCH BEGIN=============================================================

boolean FindBlack()
{ //find silver funciton start
  // scan 10x10 area starting from X and Y offset (in this case selected as x=30, y=20
  // after completing first sqaure of 10x10 pixels, offset is shifted to scan the next 10x10 square
  // 5 continuous backdots (r=0 and g=0 b=0) is taken as cofirmed black object.
  // to adjust for the noise codition testes is for RED=0 and either GREEN=0 OR BLACK=0
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

  rightm.loop();
  leftm.loop();


  Serial.print("    x_max   :");
  Serial.println(x_max);



  Serial.print("    y_max:");
  Serial.println(y_max);
  x = xoffset;

  while (x < x_max) {
    y = yoffset;
    blackcount = 0;
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

      if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100)) //  RGB for black
      { //a
        blackcount = blackcount + 1;

        black_x = x;
        black_y = y;

        if (blackcount > 3) {
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


      }//a

      y = y + 5;

    } //while loop to complete scan in y


    x = x + 5;

  } //end of scan for x-axis
  if (black_x_arr[0] > 0)
  {
    cube_x = (black_x_arr[0] + black_x_arr[1]) / 2;
    cube_y = (black_y_arr[0] + black_y_arr[1]) / 2;
    Serial.print("Sliver center (X,Y):"); Serial.print("\t");
    Serial.print(cube_x); Serial.print("\t Y :"); Serial.print("\t"); Serial.println(cube_y);
    return (true);
  }
  return (false);
} // find black function end
//BLACK SEARCH END===============================================================
//===============================================================================
//===============================================================================
//===============================================================================
//===============================================================================
//ALIGNMENT TO OBJECT BEGIN======================================================
float Align2Object(int turn)
{
  float degree;
  switch (turn)
  {

    case -1://turning left case for the turn
      {
        degree = (atan(fabs(165 - cube_x) / cube_y)) * 180 / PI;
        Serial.println(degree);
        rotate(degree, 100);
        break;
      }
    case 0:
      break;

    case 1://turning right case for ball
      {
        degree = (atan(fabs(165 - cube_x) / cube_y)) * 180 / PI;
        Serial.println(degree);
        rotate(-degree, 100);
        break;
      }
      stopMotors();
  }
  Serial.println("completed Align function");
  return (degree);
}


float Move2Object()
{
  float true_y = 207 - cube_y;
  //complicated equation based on test points to find the distance in centimeters
  float equation_y = 0.005679282788 * (true_y) * (true_y) - 0.249685611 * (true_y) + 1.4695144 + 20; //Converts y coordinates to cm(s) based on a quadratic regression formula.
  Serial.println(equation_y);
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
  rotatePixyCamAndTOF(0);//rotate to one max side
  loopSensors();//check TOF
  if (TOFdistance > MAX_WALL_DIST) //if not facing a wall
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
        if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100)) //  RGB for black
        { //a
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
  else//if facing a wall, turn the other way
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
        if ((sum_r < 100  &&  sum_g < 100 && sum_b < 100)) //  RGB for black
        { //a
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
    //Scan one half of the screen(right or left), then calculate the % of black pixels
    //Scan other half of screen and calculate % of black pixels
    //Compare halves
    //One with more Black pixels is likely the rescue zone
    //return true
  }
}
boolean detectEVAC() {

  if (leftColorData.color == WHITE && leftColorData.color == WHITE && follower.readSensors() == S1_OUT_S2_OUT)
  {
    if (rightColorData.g > WVALUE + 10 || leftColorData.g > WVALUE + 10)
    {
      return true;
    }
  }
  return false;
}
#endif
