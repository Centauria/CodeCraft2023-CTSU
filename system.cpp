//
// Created by 蔡恩光 on 2023/4/8.
//

#include "system.h"
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>

System flood_fill(CMatrix &map, bool visited[][100], Index start,
                  std::unordered_set<Index, HashFunction> &workbench_set, std::unordered_set<Index, HashFunction> &robot_set,
                  std::vector<WorkBench> &workbenches, std::vector<Robot> &robots)
{
    System system;
    system.set_shared(workbenches, robots);
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
        workbench_set.insert(w.coordinate());
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
void System::refresh()
{
    for (auto i: r_ids)
    {
        if ((*robots)[i].is_free())
        {
            processing_task.remove_if([i](Task &value) -> bool { return i == value.rid; });
        }
    }
    refreshSupply();
    refreshDemand();
    refreshPendingTask();
}

void System::decide()
{
    for (auto i: r_ids)
    {
        if ((*robots)[i].is_free() && !pending_task_list.empty())
        {
            Task task = getPendingTask(i);
            processing_task.emplace_back(task);
            (*robots)[i].path_stack.push(task.path);
        }
    }
}

Task System::getPendingTask(int robot_id)
{
    double lowest_cost = 9e9;
    Task best_task;
    for (auto &task: pending_task_list)
    {
        double cost = calculateCost(task, robot_id);
        if (lowest_cost > cost)
        {
            lowest_cost = cost;
            best_task = task;
        }
    }
    // 对pending_task_list做出相应更新
    // remove all pending task that will conflict with our new added task
    pending_task_list.remove_if([best_task](Task &pending_task) {
        return pending_task.from_id == best_task.from_id ||
               (pending_task.to_id == best_task.to_id && pending_task.item_type == best_task.item_type);
    });
    return best_task;
}
void System::set_shared(std::vector<WorkBench> &w, std::vector<Robot> &r)
{
    workbenches = &w;
    robots = &r;
}
void System::build_cache(CMatrix &map)
{
    int m = 0;
    for (auto i: w_ids)
        if (m < i) m = i;
    cost_index_map = DMatrix(m, m, []() {
        return -1;
    });
    for (auto j: w_ids)
    {
        auto me = (*workbenches)[j].coordinate();
        std::vector<Index> w_except;
        w_except.reserve(w_ids.size() - 1);
        for (auto i: w_ids)
        {
            if (i != j)
            {
                w_except.emplace_back((*workbenches)[i].coordinate());
            }
        }
        auto paths = bfs(map, me, w_except, 2);
        for (auto &p: paths)
        {
            auto terminal = p.data.back();
            for (auto i: w_ids)
            {
                if ((*workbenches)[i].coordinate() == terminal)
                {
                    path_cache.emplace_back(p);
                    cost_cache.emplace_back(p.distance());
                    cost_index_map(j, i) = path_cache.size() - 1;
                }
            }
        }
    }
}
double System::calculateCost(const Task &task, int robot_id)
{
    double cost = 0;
    auto c_id = int(cost_index_map(task.from_id, task.to_id));
    cost += cost_cache[c_id];
    auto robot_index = get_index((*robots)[robot_id].position);
    for (auto i: w_ids)
    {
        if (robot_index == (*workbenches)[i].coordinate())
        {
            cost += cost_cache[int(cost_index_map(i, task.from_id))];
            break;
        }
    }
    return cost;
}

void System::refreshSupply()
{
    std::set<int> dedup;
    for (auto &t: processing_task)
        dedup.insert(t.from_id);
    for (auto &i: w_ids)
    {
        if ((*workbenches)[i].product_status && !dedup.count(i))
        {
            supply_list.emplace_back(SD{i, (*workbenches)[i].type});
        }
    }
}

void System::refreshDemand()
{
    std::set<int> dedup[10];
    for (auto &t: processing_task)
        if ((*workbenches)[t.to_id].type != 8 && (*workbenches)[t.to_id].type != 9) dedup[t.item_type].insert(t.to_id);
    for (int16_t t = 7; t >= 1; t--)
    {
        for (auto &i: w_ids)
        {
            if ((*workbenches)[i].isFree(t) || dedup[t].count(i) || !(*workbenches)[i].needRawMaterial(t))
                continue;
            demand_list[t].emplace_back(SD{i, t});
        }
    }
}

void System::refreshPendingTask()
{
    // DONE
    for (auto &s: supply_list)
    {
        for (auto &d: demand_list[s.item_type])
        {
            int cost_index = cost_index_map(s.workbench_id, d.workbench_id);
            Task task = {-1, path_cache[cost_index], s.workbench_id, d.workbench_id, s.item_type};
        }
    }
}

void System::clear()
{
    pending_task_list.clear();
    supply_list.clear();
    for (auto &d: demand_list) d.clear();
}