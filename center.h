//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "map.h"
#include "point.h"
#include "robot.h"
#include "workbench.h"

class Center
{
public:
    Center();
    void initialize();
    bool refresh();
    void decide();
    void step();

    // Logging 相关属性
    std::string _logging_name = "center";

private:
    int currentFrame = 0;
    int deltaFrame = 0;// Difference between this frame and last frame
    double frameRate = 50;
    const size_t maps_row_num = 100;
    const size_t maps_col_num = 100;
    std::vector<std::shared_ptr<WorkBench>> workbenches;
    std::vector<std::shared_ptr<Robot>> robots;
    GameMap map{maps_row_num, maps_col_num};
};


#endif//CODECRAFTSDK_CENTER_H
