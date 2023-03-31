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
    // TODO: return true if robot p is in this region
    for (auto i: points)
    {
        if (i.x - 0.25 <= p.x && p.x <= i.x + 0.25 &&
            i.y - 0.25 <= p.y && p.y <= i.y + 0.25)
        {
            return true;
        }
    }
    return false;
}
bool Region::reachable(Point p, double detect_radius, double target_radius)
{
    // TODO: return true if workbench p is reachable by robot in this region
    for(auto i: points){
        for (int a = 1; a >= -1; a -= 2) {
            for (int b = 1; b >= -1; b -= 2) {
                //判断x+a，y+b能否够到Point p
                Point temp(i.x+a*0.25, i.y+b*0.25);
                temp = temp - p;
                if(temp.norm() < detect_radius){
                    return true;
                }
            }
        }
    }
    return false;
}
