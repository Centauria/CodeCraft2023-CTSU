//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include "delay.h"
#include "pid.h"
#include "point.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "arguments.h"

struct Action {
    double forward;
    double rotate;
};

enum Trade
{
    NONE,
    BUY,
    SELL
};


class Robot : public Object
{
public:
    Robot(int16_t id, double x, double y, Point first_point);
    std::tuple<Trade, Point> step(double delta);

    // decisions
    void forward(double v) const;
    void rotate(double w) const;
    void buy() const;
    void sell() const;
    void destroy() const;

    // commands from Center
    void add_target(Point T);
    void abort_current_target();
    void abort_all_target();
    size_t target_queue_length();
    Point target_queue_head();
    Point target_queue_tail();
    void set_obstacle(const std::vector<std::unique_ptr<Object>> &obstacles);// 设定障碍物（其他机器人）坐标

    // Calculation
    Action calculate_dynamic(double delta);
    Trade calculate_trade();

    // these functions may be useful
    double ETA();
    double ETA(const std::vector<Point> &points);
    [[nodiscard]] bool isLoaded() const;

    // 外部决定属性
    // 线速度和坐标定义均在父类中
    int16_t id;                // 机器人ID
    int16_t workbench_id;      // 所处工作台id
    int16_t item_type;         // 携带物品类型
    double time_val = 0.0;     // 时间价值系数
    double collision_val = 0.0;// 碰撞价值系数
    double omega = 0.0;        // 角速度
    double orientation;        // 朝向

    // Logging 相关属性
    std::string _logging_name = "robot";

    // 内部计算属性
    Delay position_delay{0.4};
    PIDController azimuth_derivative{0.0, 0.0, 1.0};
    PIDController azimuth_integral{0.0, 1.0, 0.0};
    PIDMatrix<2, 2> pos_angle_matrix{
            Matrix<2, 3>{
                    std::array<double, 6>{
                            {5.0, 0.000, -1.0,
                             15.0, 0.000, 0.0}}},
            Matrix<2, 2>{
                    std::array<double, 4>{
                            {1.0, 0.0,
                             0.0, 1.0}}}};

    // getter
    std::deque<Point> get_targets();

    Argument args;

private:
    // 外部设置属性
    std::deque<Point> targets;
    std::vector<Object> obstacles;
};


#endif//CODECRAFTSDK_ROBOT_H
