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
public:
    WorkBench(int16_t type, double x, double y);
    WorkBench(int16_t type, double x, double y, int product_frames_remained,
              int16_t material_status, bool product_status);
    double ETA();                              // 剩余生产时间（秒）
    [[nodiscard]] bool isFree(int index) const;// 查看指定原材料格状态
    [[nodiscard]] bool isReady() const;        // 查看产品格状态
    bool needRawMaterial(int index);           // 原材料格里是否有index这种类型的原料

    int16_t type;              // 工作台类型
    Point coordinate;           //坐标
    int product_frames_remained;// 剩余生产时间
    int16_t material_status;   // 原材料格状态
    bool product_status;        // 产品格状态
    int16_t id;
};
#endif//CODECRAFTSDK_WORKBENCH_H
