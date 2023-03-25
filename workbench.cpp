//
// Created by Centauria V. CHEN on 2023/3/11.
//

#include <cstring>

#include "workbench.h"


WorkBench::WorkBench(int16_t type, double x, double y)
{
    this->type = type;
    coordinate = {x, y};
    material_status = 0;
    product_status = false;
    product_frames_remained = -1;
}

WorkBench::WorkBench(int16_t type, double x, double y, int product_frames_remained,
                     int16_t material_status, bool product_status)
{
    this->type = type;
    coordinate = {x, y};
    this->product_frames_remained = product_frames_remained;
    this->material_status = material_status;
    this->product_status = product_status;
}

double WorkBench::ETA() const
{
    return product_status / 50.0;
}

bool WorkBench::isFree(int index) const
{
    return static_cast<bool>(material_status & (1 << index));
}

bool WorkBench::isReady() const
{
    return product_status;
}

bool WorkBench::needRawMaterial(int index) const
{
    return INGREDIENTS[type][index];
}