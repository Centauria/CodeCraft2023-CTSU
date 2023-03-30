//
// Created by Centauria V. CHEN on 2023/3/29.
//

#ifndef CODECRAFTSDK_SYSTEM_H
#define CODECRAFTSDK_SYSTEM_H

#include "point.h"
#include <cstdint>
#include <vector>

struct Node {
    uint8_t type;
};

struct Region {
    std::vector<Point> points;
};

class System
{
};

#endif//CODECRAFTSDK_SYSTEM_H
