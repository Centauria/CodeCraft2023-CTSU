//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <vector>

#include "math/point.h"
#include "robot.h"


class Center
{
public:
    Center();

private:
    int currentFrame = 0;
    std::vector<Point> workbenches;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
