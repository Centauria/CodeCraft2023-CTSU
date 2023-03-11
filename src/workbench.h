//
// Created by Centauria V. CHEN on 2023/3/11.
//

#ifndef CODECRAFTSDK_WORKBENCH_H
#define CODECRAFTSDK_WORKBENCH_H

#include "math/point.h"
#include <string>
#include <vector>


class WorkBench
{
private:
    uint8_t type;               // 工作台类型
    Point coordinate;           //坐标
    int product_frames_remained;// 剩余生产时间
    uint8_t material_status;    // 原材料格状态
    bool product_status;        // 产品格状态
public:
    WorkBench();
    WorkBench(uint8_t type, double x, double y);
    double ETA();           // 剩余生产时间
    bool is_free(int index);// 查看指定原材料格状态
};
#endif//CODECRAFTSDK_WORKBENCH_H
