//
// Created by Centauria V. CHEN on 2023/3/29.
//

#include "system.h"
#include "coordinate.h"
#include "map.h"
#include <stack>
#include "roadblocker.h"

Region::Region() = default;
void Region::add_point(Index p)
{
    pts.emplace_back(p);
}
bool Region::include(Point p)
{
    // TODO: return true if robot p is in this region
    auto index = get_index(p);
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
                auto c = get_point(i);
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
System::System() = default;
void System::add_region(const Region &region)
{
}
void System::add_robot(const Robot &robot)
{
}
void System::add_workbench(const WorkBench &workBench)
{
}

System floodfill(DMatrix &Dmap, Index index, GameMap &map, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches)
{
    System system;
    std::stack<Index> s;
    s.push(index);
    while (!s.empty())
    {
        Index t = s.top();
        s.pop();
        auto p = get_point(t);
        if ('1' <= map(t.y, t.x) && map(t.y, t.x) <= '9')
        {
            for (auto &workbench: workbenches)
            {
                if (workbench->coordinate == p)
                {
                    system.add_workbench(*workbench);
                }
            }
        } else if (map(t.y, t.x) == 'A')
        {
            for (auto &robot: robots)
            {
                if (robot->position == p)
                {
                    system.add_robot(*robot);
                }
            }
        }
        for (int i = 1; i >= -1; i -= 2)
        {
            if (t.y + i > -1 && t.y + i < 100)
                if (Dmap(t.y + i, t.x) > 0)
                {
                    s.push({t.y + i, t.x});
                }
            if (t.x + i > -1 && t.x + i < 100)
                if (Dmap(t.y, t.x + i) > 0)
                {
                    s.push({t.y, t.x + i});
                }
        }
        Dmap(t.y, t.x) = -2;// -2 是洪水
    }
    return system;
}

std::vector<System> get_systems(GameMap map, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches)
{
    std::vector<System> systems;
    DMatrix Dmap = map.get_distances();
    RoadBlocker roadblocker;
    roadblocker.block_narrow_road(Dmap);
    for (int i = 1; i <= 100; i++)
    {
        for (int j = 1; j <= 100; j++)
        {
            if (Dmap(j, i) > 0) continue;// 如果不是墙 0，不是路障 -1，也不是洪水的话 -2
            System system = floodfill(Dmap, Index{j, i}, map, robots, workbenches);
            systems.emplace_back(system);
        }
    }
    return systems;
}