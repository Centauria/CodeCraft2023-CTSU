//
// Created by Centauria V. CHEN on 2023/4/7.
//

#ifndef CODECRAFTSDK_CONCEPT_H
#define CODECRAFTSDK_CONCEPT_H

#include "Vector2D.h"
#include "function.h"
#include "path.h"
#include <cstdint>
#include <stack>

struct WorkBench {
    int type;                   // 工作台类型
    Point position;             // 坐标
    int product_frames_remained;// 剩余生产时间
    int material_status;        // 原材料格状态
    bool product_status;        // 产品格状态
    [[nodiscard]] Index coordinate() const
    {
        return get_index(position);
    }
};

struct Robot {
    int id;                    // 机器人ID
    int workbench_id;          // 所处工作台id
    int item_type;             // 携带物品类型
    double time_val = 0.0;     // 时间价值系数
    double collision_val = 0.0;// 碰撞价值系数
    double omega = 0.0;        // 角速度
    double orientation;        // 朝向
    Point position;
    Velocity velocity;

    void step(double delta, CMatrix &map);
    Vector2D action(const Path &path, CMatrix &map);
    std::vector<double> observe(CMatrix &map, size_t y_n = 5, size_t x_n = 9) const;
    [[nodiscard]] double forward_correction(const std::vector<double> &obs) const;
    [[nodiscard]] double rotate_correction(const std::vector<double> &obs) const;

    double progress();
    double progress(const Path &path);
    bool running = true;

    [[nodiscard]] double radius() const;
    [[nodiscard]] bool is_free() const;

    std::stack<Path> path_stack;
};

struct Timer {
    int current_frame = 0;
    int delta = 0;
};

#endif//CODECRAFTSDK_CONCEPT_H
