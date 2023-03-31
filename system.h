//
// Created by Centauria V. CHEN on 2023/3/29.
//

#ifndef CODECRAFTSDK_SYSTEM_H
#define CODECRAFTSDK_SYSTEM_H

#include "coordinate.h"
#include "point.h"
#include <cstdint>
#include <vector>

struct Node {
    uint8_t type;
};

class Region
{
public:
    Region();
    void add_point(Index p);
    bool include(Point p);
    bool reachable(Point p, double detect_radius, double target_radius);


private:
    std::vector<Index> pts;
};

class System
{
public:
    System();

private:
    std::vector<Region> regions;
};

#endif//CODECRAFTSDK_SYSTEM_H
