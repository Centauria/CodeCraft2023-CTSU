//
// Created by Centauria V. CHEN on 2023/3/11.
//

#include "workbench.h"


WorkBench::WorkBench(uint8_t id, uint8_t type, double x, double y)
{
    this->id = id;
    this->type = type;
    coordinate = {x, y};
    material_status = 0;
    product_status = false;
}

WorkBench::WorkBench(uint8_t type, double x, double y, int product_frames_remained,
                     uint8_t material_status, bool product_status)
{
    this->type = type;
    coordinate = {x, y};
    this->product_frames_remained = product_frames_remained;
    this->material_status = material_status;
    this->product_status = product_status;
}

double WorkBench::ETA()
{
    return 0;
}

bool WorkBench::isFree(int index) const
{
    return static_cast<bool>(material_status & (1 << index));
}


uint8_t WorkBench::getType() const
{
    return type;
}

Point WorkBench::getCoordinate()
{
    return coordinate;
}

bool WorkBench::isReady() const
{
    return product_status;
}