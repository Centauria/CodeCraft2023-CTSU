//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include "math/point.h"
#include <cstdint>
#include <vector>

class Robot
{
public:
    Robot(uint8_t id, double x, double y, double orientation);
    Robot(double x, double y);
    void step();

    // decisions
    void forward(double v);
    void rotate(double w);
    void buy();
    void sell();
    void destroy();

    // compulsory attribute modification
    void set_position(const Point &p);
    void set_orientation(double angle);

    // commands from Center
    void set_target(Point T);                              // 设定目标点
    void set_obstacle(const std::vector<Point> &obstacles);// 设定障碍物（其他机器人）坐标

    // Calculation
    void calculate_dynamic();
    void calculate_trade();

    // these functions may be useful
    double ETA();

private:
    uint8_t id;                // 机器人ID
    uint8_t item_type;         // 携带物品类型
    double time_val = 0.0;     // 时间价值系数
    double collision_val = 0.0;// 碰撞价值系数
    double rotate_speed = 0.0; // 角速度
    double forward_speed = 0.0;// 线速度
    Velocity linearVelocity;   // 帧中线速度
    double orientation;        // 朝向
    Point coordinate;          // 坐标类

    Point target;
};


#endif//CODECRAFTSDK_ROBOT_H
