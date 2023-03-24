//
// Created by Centauria V. CHEN on 2023/3/22.
//
#include "task.h"
#include <cstring>
#include <set>

TaskManager::TaskManager()
{
    memset(item_occur_cnt, 0, sizeof(item_occur_cnt));
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
        Task task;
        while (true)
        {
            task = getPendingTask(robot->id, robots, workbenches);
            if (task.wpo_from == Point{0, 0} || task.wpo_to == Point{0, 0})
                break;
            task.status = STARTING;
            if (pending_task_list.empty())
                break;
            int16_t robot_id = checkRobotTaskTail(task.wpo_from, robots);
            if (robot_id != -1 && time_remain > 40)
            {
                task.robot_id = robot_id;
                task_list.push_back(task);
                robots[robot_id]->add_target(task.wpo_from);
                robots[robot_id]->add_target(task.wpo_to);
                item_occur_cnt[workbenches[task.wid_to]->type]++;
            } else
            {
                break;
            }
        }
        if (task.wpo_from == Point{0, 0} || task.wpo_to == Point{0, 0})
            continue;
        task.robot_id = robot->id;
        task_list.push_back(task);
        robot->add_target(task.wpo_from);
        robot->add_target(task.wpo_to);
        item_occur_cnt[workbenches[task.wid_to]->type]++;
    }
}

Task TaskManager::getPendingTask(int robot_id, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // TODO: 让robot匹配到合适的Task
    double lowest_cost = 9999999999;
    Task best_task;
    for (auto &task: pending_task_list)
    {
        Vector2D dist = task.wpo_from - robots[robot_id]->position;
        double cost = (task.dist + dist.norm()) / (task.profit / 3000);
        if (workbenches[task.wid_to]->product_frames_remained != -1) cost += 10;
        // 如果Demand工作台啥材料都没有就放放等之后再给他喂材料
        if (workbenches[task.wid_to]->material_status == 0) cost += 10;
        //         1，2，3保持持平状态，并且4，5，6也保持持平状态（图二）（图四）👇
        if (4 <= workbenches[task.wid_to]->type && workbenches[task.wid_to]->type <= 6)
        {
            int avg = (item_occur_cnt[4] + item_occur_cnt[5] + item_occur_cnt[6]) / 3;
            cost += 4 * (item_occur_cnt[workbenches[task.wid_to]->type] - avg);
        }
        if (workbenches[task.wid_to]->type == 9) cost += 6;
        std::vector<Point> ETCT;// Estimated Time to Complete the Task
        ETCT.push_back(task.wpo_from);
        ETCT.push_back(task.wpo_to);
        if (lowest_cost > cost && robots[robot_id]->ETA(ETCT) < time_remain)
        {
            lowest_cost = cost;
            best_task = task;
            task.cost = cost;
        }
        ETCT.clear();
    }
    // 对task_list做出相应更新
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
            task.wpo_from = workbenches[s.workbench_id]->coordinate;
            task.wpo_to = workbenches[d.workbench_id]->coordinate;
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
        if (t.status != OVER && workbenches[t.wid_to]->type != 8 && workbenches[t.wid_to]->type != 9) dedup[t.item_type].insert(t.wid_to);
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
            if (workbench_point == task.wpo_from)
            {
                task.status = PROCESSING;
            }
            break;
        case SELL:
            if (workbench_point == task.wpo_to && robot_id == task.robot_id)
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

void TaskManager::set_sec_remain(int Frame)
{
    time_remain = (9000 - Frame) / 50.0;
}

int16_t TaskManager::checkRobotTaskTail(Point x, const std::vector<std::unique_ptr<Robot>> &robots)
{
    for (auto &robot: robots)
    {
        if (robot->target_queue_length() == 0)
            continue;
        if (x == robot->target_queue_tail())
            return robot->id;
    }
    return -1;
}