//
// Created by 蔡恩光 on 2023/4/5.
//

#include "task.h"
#include <set>


void TaskManager::distributeTask(int16_t robot_id, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches, GameMap &map)
{
    Task best_task;
    Path best_paths[2];
    double current_aggregated_index = 0;
    for (auto s: supply_list)
    {
        Path a = getPath(robots[robot_id]->position, workbenches[s.workbench_id]->coordinate, workbenches, map, 2);
        if (a.empty()) continue;
        for (auto d: demand_list[s.item_type])
        {
            Path b = getPath(workbenches[s.workbench_id]->coordinate, workbenches[d.workbench_id]->coordinate, workbenches, map, 3);
            if (a.empty() || b.empty()) continue;
            double aggregated_index = calculateAggregatedIndex(a, b, profit[s.item_type]);
            if (aggregated_index > current_aggregated_index)
            {
                best_task = Task{s.workbench_id, d.workbench_id, aggregated_index, robot_id, s.item_type, workbenches[s.workbench_id]->coordinate, workbenches[d.workbench_id]->coordinate, PENDING};
                current_aggregated_index = aggregated_index;
                best_paths[0] = a;
                best_paths[1] = b;
            }
        }
    }
    if (current_aggregated_index == 0) return;
    //supply demand删掉该删的两个点
    supply_list.remove_if([best_task](SD &value) -> bool { return value.workbench_id == best_task.wid_from; });
    demand_list[best_task.item_type].remove_if([best_task](SD &value) -> bool { return value.workbench_id == best_task.wid_to; });
    //把best Task布置给robot
    best_task.status = STARTING;
    best_task.robot_id = robot_id;
    task_list.emplace_back(best_task);
    for (auto &path: best_paths)
    {
        for (auto &p: path)
        {
            robots[robot_id]->add_target(p);
        }
    }
}
double TaskManager::calculateAggregatedIndex(Path &a, Path &b, double gross_income)
{
    auto total_dist = double(a.size() + b.size());
    return gross_income / total_dist;
}
void TaskManager::refreshSupply(const std::vector<std::shared_ptr<WorkBench>> &workbenches)
{
    std::set<int16_t> dedup;
    for (auto &t: task_list)
        if (t.status == STARTING || t.status == PENDING) dedup.insert(t.wid_from);
    for (auto &w: workbenches)
    {
        if (w->isReady() && !dedup.count(w->id))
        {
            supply_list.emplace_back(SD{w->id, w->type, w->type});
        }
    }
}
void TaskManager::refreshDemand(const std::vector<std::shared_ptr<WorkBench>> &workbenches)
{
    std::set<int16_t> dedup[10];
    for (auto &t: task_list)
        if (t.status != OVER && workbenches[t.wid_to]->type != 8 && workbenches[t.wid_to]->type != 9) dedup[t.item_type].insert(t.wid_to);
    for (int16_t t = 7; t >= 1; t--)
    {
        for (auto &w: workbenches)
        {
            if (w->isFree(t) || dedup[t].count(w->id) || !w->needRawMaterial(t))
                continue;
            demand_list[t].emplace_back(SD{w->id, w->type, t});
        }
    }
}
Path TaskManager::getPath(Point &a, Point &b, const std::vector<std::shared_ptr<WorkBench>> &workbenches, GameMap &map, int width)
{
    Index index_a = get_index(a), index_b = get_index(b);
    int16_t id_a = -1, id_b = -1;
    for (auto &w: workbenches)
    {
        if (index_a == get_index(w->coordinate)) id_a = w->id;
        if (index_b == get_index(w->coordinate)) id_b = w->id;
    }
    if (id_a != -1 && id_b != -1)
    {
        if (path_between_workbenches[id_a][id_b].empty())
            path_between_workbenches[id_a][id_b] = a_star(map, index_a, index_b, width);
        return path_between_workbenches[id_a][id_b];
    } else
    {
        return a_star(map, index_a, index_b, width);
    }
}
void TaskManager::refreshTaskStatus(Trade action, int16_t robot_id)
{
    for (auto t: task_list)
    {
        if (robot_id == t.robot_id)
        {
            switch (action)
            {
            case BUY:
                t.status = PROCESSING;
                return;
            case SELL:
                t.status = OVER;
                return;
            case NONE:
                return;
            }
        }
    }
}
void TaskManager::freeSupplyDemandList()
{
    supply_list.clear();
    for (auto &i: demand_list) i.clear();
}
bool TaskManager::haveTask()
{
    if (supply_list.empty())
        return false;
    for (auto &i: demand_list)
    {
        if (!i.empty())
            return true;
    }
    return false;
}
