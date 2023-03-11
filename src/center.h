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
    Vector2D coordinate;          //坐标
    int reaminsProductFrames; // 剩余生产时间
    int materials;            // 原材料格状态
    bool product_status;      // 产品格状态
public:
    WrokBench(/* args */);
    ~WrokBench();
    void setType(int type);
    int getType();
    void setCoordinate(Vector2D coordinate);
    Vector2D getCoordinate();
    void setMaterials(int materials);
    int getMaterials();
    void setProduct_status(bool product_status);
    bool getProduct_status();
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
    // std::vector<Vector2D> workbenches_coordinate;
    std::vector<WrokBench> WrokBenchs;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
