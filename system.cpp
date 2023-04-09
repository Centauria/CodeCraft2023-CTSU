//
// Created by 蔡恩光 on 2023/4/8.
//

#include "system.h"
#include <cstring>
#include <iostream>
#include <queue>
#include <unordered_set>

System flood_fill(CMatrix &map, bool visited[][100], Index start,
                  std::unordered_set<Index, HashFunction> &workbench_set, std::unordered_set<Index, HashFunction> &robot_set,
                  std::vector<WorkBench> &workbenches, std::vector<Robot> &robots)
{
    System system;
    std::queue<Index> q;
    q.push(start);
    visited[start.y][start.x] = true;
    while (!q.empty())
    {
        Index cur = q.front();
        q.pop();
        if (workbench_set.count({cur.y, cur.x}))
        {
            for (int i = 0; i < workbenches.size(); ++i)
            {
                if (workbenches[i].coordinate() == cur)
                {
                    system.w_ids.emplace_back(i);
                }
            }
        }
        if (robot_set.count({cur.y, cur.x}))
        {
            for (int i = 0; i < robots.size(); ++i)
            {
                if (get_index(robots[i].position) == cur)
                {
                    system.r_ids.emplace_back(i);
                }
            }
        }
        //extend
        for (int i = -1; i <= 1; i += 2)
        {
            Index a(cur.y + i, cur.x), b(cur.y, cur.x + i);
            if (accessible(a, map, 2) && map(a.y, a.x) == 3 && !visited[a.y][a.x])
            {
                q.push(a);
                visited[a.y][a.x] = true;
            }
            if (accessible(b, map, 2) && map(b.y, b.x) == 3 && !visited[b.y][b.x])
            {
                q.push(b);
                visited[b.y][b.x] = true;
            }
        }
    }
    return system;
}

std::vector<System> get_system(CMatrix &map, std::vector<WorkBench> &workbenches, std::vector<Robot> &robots)
{
    std::unordered_set<Index, HashFunction> workbench_set;
    std::unordered_set<Index, HashFunction> robot_set;
    for (auto &r: robots)
        robot_set.insert(get_index(r.position));
    for (auto &w: workbenches)
        robot_set.insert(w.coordinate());
    std::vector<System> systems;
    bool visited[100][100];
    memset(visited, false, sizeof(visited));
    for (int j = 0; j < 100; j++)
    {
        for (int i = 0; i < 100; i++)
        {
            if (visited[j][i] || map(j, i) == 0 || !accessible({j, i}, map, 2)) continue;
            systems.emplace_back(flood_fill(map, visited, Index{j, i}, workbench_set, robot_set, workbenches, robots));
        }
    }
    return systems;
}