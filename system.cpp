//
// Created by Centauria V. CHEN on 2023/3/29.
//

#include "system.h"
#include "coordinate.h"
#include "map.h"
#include <stack>

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

void block_all_roads(DMatrix &Dmap)
{
    //TODO: lamda表达式卷积还没写
    //TODO: hhhh不想写了要不就用这依托答辩吧（doge）史上最强6重for循环
    for (int j = 1; j < 99; j++)
    {
        for (int i = 1; i < 99; i++)// j,i 是原始障碍物
        {
            if (Dmap(j, i) != 0) continue;
            for (int jj = j - 1; jj <= j + 1; jj++)
            {
                for (int ii = i - 1; ii <= i + 1; ii++)// jj,ii 是原始障碍物外面那一圈玩意
                {
                    if (jj == j && ii == i) continue;
                    if (jj == 0 || jj == 99 || ii == 0 || ii == 99)
                    {
                        Dmap(jj, ii) = -1;
                        continue;
                    }
                    for (int jjj = jj - 1; jjj <= jj + 1; jjj++)
                    {
                        for (int iii = ii - 1; iii <= ii + 1; iii++)
                        {
                            if ((jjj - j == 2 || iii - i == 2) && Dmap(jjj, iii) == 0)
                            {
                                Dmap(jjj, iii) = -1;
                            }
                        }
                    }
                }
            }
        }
    }
}

System floodfill(DMatrix &Dmap, Index index, Map &map, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{//TODO:设置robot和workbench的参数
    System system;
    std::stack<Index> s;
    s.push(index);
    while (!s.empty())
    {
        Index t = s.top();
        s.pop();
        Point p;
        p.x = t.x * 0.5 + 0.25;
        p.y = t.y * 0.5 + 0.25;
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

std::vector<System> get_systems(Map map, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    std::vector<System> systems;
    DMatrix Dmap = map.get_distances();
    block_all_roads(Dmap);
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            if (Dmap(j, i) > 0) continue;// 如果不是墙 0，不是路障 -1，也不是洪水的话 -2
            System system = floodfill(Dmap, Index{j, i}, map, robots, workbenches);
            systems.emplace_back(system);
        }
    }
    return systems;
}