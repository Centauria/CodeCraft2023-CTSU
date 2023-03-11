//
// Created by Centauria V. CHEN on 2023/3/11.
//

#include "workbench.h"


WorkBench::WorkBench(uint8_t type, double x, double y)
{
    this->type = type;
    coordinate = {x, y};
    material_status = 0;
    product_status = false;
}
double WorkBench::ETA()
{
    return 0;
}

bool WorkBench::is_free(int index)
{
    // TODO: 得到指定index的原材料格状态
    return false;
}


uint8_t WorkBench::getType() const
{
    return type;
}

Vector2D WorkBench::getCoordinate()
{
    return coordinate;
}

bool WorkBench::is_ready() const
{
    return product_status;
}