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

class Region
{
public:
    Region();
    void add_point(Point p);
    bool include(Point p);
    bool reachable(Point p, double detect_radius, double target_radius);


private:
    std::vector<Point> points;
};

class System
{
public:
    System();

private:
    std::vector<Region> regions;
};

#endif//CODECRAFTSDK_SYSTEM_H
