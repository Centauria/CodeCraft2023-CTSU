//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <string>
#include <vector>

#include "math/point.h"
#include "robot.h"

class WrokBench
{
private:
    int type;                 // 工作台类型
    Point coordinate;          //坐标
    int reaminsProductFrames; // 剩余生产时间
    int materials;            // 原材料格状态
    bool product_status;      // 产品格状态
public:
    WrokBench(/* args */);
    ~WrokBench();
};



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
    // std::vector<Point> workbenches;
    std::vector<Vector2D> workbenches_coordinate;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
