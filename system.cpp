//
// Created by Centauria V. CHEN on 2023/3/29.
//

#include "system.h"

Region::Region() = default;
void Region::add_point(Point p)
{
    points.emplace_back(p);
}
bool Region::include(Point p)
{
    // TODO: return true if p in this region
    return false;
}
bool Region::reachable(Point p)
{
    // TODO: return true if p is reachable by robot in this region
    return false;
}
