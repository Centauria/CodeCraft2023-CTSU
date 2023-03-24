//
// Created by Centauria V. CHEN on 2023/3/22.
//
#include "task.h"
#include <set>

TaskManager::TaskManager()
{
}

void TaskManager::set_adj_matrix(const std::vector<Point> &workbench_position)
{
    memset(adj_matrix, 0, sizeof(adj_matrix));
    for (int i = 0; i < workbench_position.size(); i++)
    {
        for (int j = i + 1; j < workbench_position.size(); j++)
        {
            adj_matrix[i][j] = adj_matrix[j][i] = (workbench_position[i] - workbench_position[j]).norm();
        }
    }
}

void TaskManager::distributeTask(const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // DONE
    for (auto &robot: robots)
    {
        if (robot->target_queue_length() != 0 || pending_task_list.empty())
        {// EXPLAIN：如果robot还有任务在身 或者 没有能接的任务 就不接任务。
            continue;
        }
        // 接任务👇
        Task task = getPendingTask(robot->id, robots, workbenches);
        task.robot_id = robot->id;
        task.status = STARTING;
        task_list.push_back(task);
        robot->add_target(workbenches[task.wid_from]->coordinate);
        robot->add_target(workbenches[task.wid_to]->coordinate);
    }
}

Task TaskManager::getPendingTask(int robot_id, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // TODO: 让robot匹配到合适的Task
    double lowest_cost = 9999999999;
    Task best_task;
    for (auto &task: pending_task_list)
    {
        Vector2D dist = workbenches[task.wid_from]->coordinate - robots[robot_id]->position;
        double cost = (task.dist + dist.norm()) / (task.profit / 3000);
        if (workbenches[task.wid_to]->product_frames_remained != -1) cost += 10;
        // 如果Demand工作台啥材料都没有就放放等之后再给他喂材料
        if (workbenches[task.wid_to]->material_status == 0) cost += 7.5;
        task.cost = cost;
        if (lowest_cost > cost)
        {
            lowest_cost = cost;
            best_task = task;
        }
    }
    // TODO: 对task_list做出相应更新
    // remove all pending task that will conflict with our new added task
    pending_task_list.remove_if([best_task](Task &pending_task) {
        return pending_task.wid_from == best_task.wid_from ||
               (pending_task.wid_to == best_task.wid_to && pending_task.item_type == best_task.item_type);
    });
    return best_task;
}
void TaskManager::refreshPendingTask(const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // DONE
    refreshSupply(workbenches);
    refreshDemand(workbenches);
    while (!supply_list.empty())
    {
        SD s = supply_list.front();
        supply_list.pop();
        for (auto d: demand_list[s.item_type])
        {
            Task task;
            task.wid_from = s.workbench_id;
            task.wid_to = d.workbench_id;
            task.status = PENDING;
            task.profit = profit[s.item_type];
            task.dist = adj_matrix[s.workbench_id][d.workbench_id];
            task.item_type = s.item_type;
            pending_task_list.push_back(task);
        }
    }
    freeSupplyDemandList();
}

void TaskManager::refreshSupply(const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // DONE
    std::set<int16_t> dedup;
    for (auto &t: task_list)
    {
        if (t.status == STARTING || t.status == PENDING) dedup.insert(t.wid_from);
    }
    for (auto &w: workbenches)
    {
        if (w->isReady() && !dedup.count(w->id))
        {
            supply_list.emplace(SD{w->id, w->type, w->type});
        }
    }
    dedup.clear();
}

void TaskManager::refreshDemand(const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // DONE
    std::set<int16_t> dedup[10];
    for (auto &t: task_list)
    {
        if (t.status != OVER) dedup[t.item_type].insert(t.wid_to);
    }
    for (int16_t t = 7; t >= 1; t--)
    {
        for (auto &w: workbenches)
        {
            if (w->isFree(t))
                continue;
            if (!w->needRawMaterial(t))
                continue;
            if (dedup[t].count(w->id))
                continue;
            demand_list[t].emplace_back(SD{w->id, w->type, t});
        }
    }
    for (auto &i: dedup)
    {
        i.clear();
    }
}

void TaskManager::freeSupplyDemandList()
{
    for (auto &i: demand_list)
    {
        i.clear();
    }
}

void TaskManager::refreshTaskStatus(int robot_id, Trade action, Point workbench_point, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    if (action == NONE)
        return;
    for (auto &task: task_list)
    {
        switch (action)
        {
        case BUY:
            if (workbench_point == workbenches[task.wid_from]->coordinate)
            {
                task.status = PROCESSING;
            }
            break;
        case SELL:
            if (workbench_point == workbenches[task.wid_to]->coordinate && robot_id == task.robot_id)
            {
                task.status = OVER;
            }
            break;
        default:
            break;
        }
    }
}

void TaskManager::clearPendingTaskList()
{
    pending_task_list.clear();
}

void TaskManager::clearOverTask()
{
    task_list.remove_if([](auto x) { return x.status == OVER; });
}