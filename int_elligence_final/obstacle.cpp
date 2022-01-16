#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "global.h"
#include "utils.h"
bool detectObstacles()
{
    if(TOFdistance < 50)
    {
        rotatePixyCamAndTOF(0);
        
        return true;
    }
    return false;
}
bool doObstacles()
{
  return true;
}
#endif
