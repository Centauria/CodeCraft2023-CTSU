//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <string>
#include <vector>

#include "math/point.h"
#include "robot.h"
#include "workbench.h"


class Center
{
public:
    Center();
    void initialize();
    bool refresh();
    void step();

private:
    int currentFrame = 0;
    int deltaFrame = 0;// Difference between this frame and last frame
    std::vector<WorkBench> workbenches;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
