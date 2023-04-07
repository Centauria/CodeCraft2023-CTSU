//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include "arguments.h"
#include "map.h"
#include "point.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

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
    Robot(int16_t id, double x, double y);
    std::tuple<Trade, Point> step(double delta);

    // decisions
    void forward(double v) const;
    void rotate(double w) const;
    void buy() const;
    void sell() const;
    void destroy() const;

    // commands from Center
    void set_map(GameMap &map);
    void add_target(Index T);
    void add_path(const Path &path);
    void abort_current_target();
    void abort_all_target();
    size_t target_queue_length();
    Index target_queue_head();
    Index target_queue_tail();
    void set_obstacle(const std::vector<std::unique_ptr<Object>> &obstacles);// 设定障碍物（其他机器人）坐标

    // Calculation
    Action calculate_dynamic(double delta);
    Trade calculate_trade();
    void analyze_track();
    std::vector<double> target_distance();
    bool goal();

    // these functions may be useful
    double ETA();
    double ETA(const std::vector<Point> &points);
    [[nodiscard]] bool isLoaded() const;
    [[nodiscard]] double radius() const;

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
    size_t target_distance_cache_length = 2;

    // getter
    std::deque<Index> get_targets();
    std::tuple<double, double> see(double field_angle = M_PI_2, size_t ray_count = 20, double ray_length = 0.9, size_t ray_division = 9);

private:
    std::deque<Index> targets;
    std::vector<Object> obstacles;
    std::shared_ptr<GameMap> gameMap;

    // cache
    std::vector<double> distance_cache;
};


#endif//CODECRAFTSDK_ROBOT_H
