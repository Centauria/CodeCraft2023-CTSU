//
// Created by Centauria V. CHEN on 2023/4/7.
//

#ifndef CODECRAFTSDK_CONCEPT_H
#define CODECRAFTSDK_CONCEPT_H

#include "Vector2D.h"
#include <cstdint>

struct WorkBench {
    int type;                   // 工作台类型
    Index coordinate;           //坐标
    int product_frames_remained;// 剩余生产时间
    int material_status;        // 原材料格状态
    bool product_status;        // 产品格状态
    int id;
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
};

#endif//CODECRAFTSDK_CONCEPT_H
