//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include <cstdint>
#include "math/point.h"

class Robot
{
public:
    explicit Robot(uint8_t id);
    void step();
    void forward(double v);
    void rotate(double w);
    void buy();
    void sell();
    void destroy();

    // these functions may be useful
    double ETA();

private:
    uint8_t id;         // 所处工作台ID
    int items_type;               // 携带物品类型
    double coefficientOfTimeValue=0.0; // 时间价值系数
    double collisionValueFactor=0.0;   // 碰撞价值系数
    double rotate_speed = 0.0;      //  角速度
    double forward_speed = 0.0;     // 线速度
    Point linearVelocity;           // 帧中线速度
    double orientation;            // 朝向
    Point coordinate;               // 坐标类

    // 请根据实际情况添加所需属性和getter、setter
};


#endif//CODECRAFTSDK_ROBOT_H
