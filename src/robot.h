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
    Robot(int16_t id, double x, double y);
    void step(double delta);

    // decisions
    void forward(double v) const;
    void rotate(double w) const;
    void buy() const;
    void sell() const;
    void destroy() const;

    // commands from Center
    void set_target(Point T);                              // 设定目标点
    void set_obstacle(const std::vector<Point> &obstacles);// 设定障碍物（其他机器人）坐标

    // Calculation
    Action calculate_dynamic(double delta);
    void calculate_trade();

    // these functions may be useful
    double ETA();

    // 外部决定属性
    int16_t id;                // 机器人ID
    int16_t workbench_id;      // 所处工作台id
    int16_t item_type;         // 携带物品类型
    double time_val = 0.0;      // 时间价值系数
    double collision_val = 0.0; // 碰撞价值系数
    double omega = 0.0;         // 角速度
    Velocity velocity{};        // 线速度
    double orientation;         // 朝向
    Point coordinate;           // 坐标类

    // Logging 相关属性
    std::string _logging_name = "robot";

    // 内部计算属性
    PIDController position_error{1.0, 0.4, 0.0};
    PIDController angle_error{1.2, 0.4, 0.0};

private:
    // 外部设置属性
    Point target;
};


#endif//CODECRAFTSDK_ROBOT_H
