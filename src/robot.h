//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include <cstdint>

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
    double coefficientOfTimeValue; // 时间价值系数
    double collisionValueFactor;   // 碰撞价值系数
    double rotate_speed = 0.0;      //  角速度
    double forward_speed_x = 0.0;   // 线速度 x
    double forward_speed_y = 0.0;    // 线速度 y
    double orientation;            // 朝向
    double coordinate_x;           // 坐标 x
    double coordinate_y;           // 坐标 y
    
    // 请根据实际情况添加所需属性和getter、setter
};


#endif//CODECRAFTSDK_ROBOT_H
