//
// Created by Centauria V. CHEN on 2023/3/29.
//

#include "system.h"
#include "coordinate.h"

Region::Region() = default;
void Region::add_point(Index p)
{
    pts.emplace_back(p);
}
bool Region::include(Point p)
{
    // TODO: return true if robot p is in this region
    auto index = discrete(p);
    return std::any_of(pts.cbegin(), pts.cend(), [index](auto v) {
        return v == index;
    });
}
bool Region::reachable(Point p, double detect_radius, double target_radius)
{
    // TODO: return true if workbench p is reachable by robot in this region
    for (auto i: pts)
    {
        for (int a = 1; a >= -1; a -= 1)
        {
            for (int b = 1; b >= -1; b -= 1)
            {
                //判断x+a，y+b能否够到Point p
                auto c = center(i);
                Point temp(c.x + a * 0.25, c.y + b * 0.25);
                temp = temp - p;
                if (temp.norm() < detect_radius)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
