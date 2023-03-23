//
// Created by Centauria V. CHEN on 2023/3/22.
//
#include <set>
#include "task.h"

TaskManager::TaskManager(size_t n_workbenches, size_t n_robots)
{
    
}
void TaskManager::distributeTask(std::vector<std::unique_ptr<Robot>> robots, std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // DONE
    for (const auto &robot: robots)
    {
        if (robot->target_queue_length() != 0)
        {// EXPLAIN：如果robot还有任务在身就不接任务。
            continue;
        }
        Task task = getPendingTask(robot->id, robots, workbenches);
        task.robot_id = robot->id;
        task_list.push_back(task);
        robot->add_target(workbenches[task.wid_from]->coordinate);
        robot->add_target(workbenches[task.wid_to]->coordinate);
        // remove all pendingtask that will conflict with our new added task
        pending_task_list.remove_if([this, task](const Task& pendingtask){return conflict(pendingtask, task);});
    }
}

Task TaskManager::getPendingTask(int robot_id, std::vector<std::unique_ptr<Robot>> robots, std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // TODO: 让robot匹配到合适的Task
    double lowest_cost = 999999;
    Task best_task;
    for (auto task: pending_task_list)
    {
        Point dist = workbenches[task.wid_from]->coordinate - robots[robot_id]->position;
        double cost = (task.dist + dist.norm()) / (task.profit / 3000);
        if (lowest_cost > cost)
        {
            lowest_cost = cost;
            best_task = task;
        }
    }
    // TODO: 对task_list做出相应更新
    return best_task;
}
void TaskManager::refreshPendingTask(std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // DONE
    refreshSupply(workbenches);
    refreshDemand(workbenches);
    while (supply_list.size())
    {
        Supply s = supply_list.front();
        supply_list.pop();
        if (demand_list[s.item_type].empty())
            continue;
        for (auto d: demand_list[s.item_type])
        {
            Task task;
            task.wid_from = s.workbench_id;
            task.wid_to = d.workbench_id;
            task.status = PENDING;
            pending_task_list.push_back(task);
        }
    }
    freeSupplyDemandList();
}

void TaskManager::refreshSupply(std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // DONE
    std::set<int16_t> dedup;
    for(auto t: task_list){
        if(t.status == STARTING) dedup.insert(t.wid_from);
    }
    for (auto &w: workbenches)
    {
        if (w->isReady() && !dedup.count(w->id))
        {
            Supply s;
            s.item_type = w->type;
            s.workbench_id = w->id;
            s.item_type = w->type;
            supply_list.push(s);
        }
    }
    dedup.clear();
}

void TaskManager::refreshDemand(std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // DONE
    std::set<int> dedup[10];
    for(auto t: task_list){
        if(t.status == STARTING || t.status == PROCESSING) dedup[t.item_type].insert(t.wid_to);
    }
    for (int t = 7; t >= 1; t--)
    {
        for (auto &w: workbenches)
        {
            if (w->isFree(t))
                continue;
            if (!w->needRawMaterial(t))
                continue;
            if (dedup[t].count(w->id))
                continue;
            Demand d;
            d.workbench_id = w->id;
            d.workbrench_type = w->type;
            d.item_type = t;
            demand_list[t].push_back(d);
        }
    }
    return;
}

void TaskManager::freeSupplyDemandList()
{
    //DONE
    while (!supply_list.empty())
    {
        supply_list.pop();
    }
    for (int i = 0; i < 10; i++)
    {
        demand_list[i].clear();
    }
}

bool TaskManager::conflict(const Task& pendingtask, Task task)
{
    // DONE...Hopefully
    if(pendingtask.wid_from == task.wid_from) return true;
    if(pendingtask.wid_to == task.wid_to && pendingtask.item_type == task.item_type) return true;
    return false;
}

void TaskManager::refreshTaskStatus(){

}