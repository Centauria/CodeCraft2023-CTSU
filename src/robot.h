//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include "math/pid.h"
#include "math/point.h"
#include <cmath>
#include <cstdint>
#include <vector>

struct Action {
    double forward;
    double rotate;
};

class Robot
{
public:
    Robot(uint8_t id, double x, double y);
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
    Action calculate_dynamic();
    void calculate_trade();

    // these functions may be useful
    double ETA();

private:
    // 外部决定属性
    uint8_t id;                 // 机器人ID
    uint8_t workBench_id;       // 所处工作台id
    uint8_t item_type;          // 携带物品类型
    double time_val = 0.0;      // 时间价值系数
    double collision_val = 0.0; // 碰撞价值系数
    double omega = 0.0;         // 角速度
    Velocity velocity{};        // 线速度
    double orientation = M_PI_2;// 朝向
    Point coordinate;           // 坐标类

    // 外部设置属性
    Point target;

    // 内部计算属性
    PIDController position_error{0.5, 0.05, 0.25};
    PIDController angle_error{0.5, 0.05, 0.15};
};


#endif//CODECRAFTSDK_ROBOT_H
