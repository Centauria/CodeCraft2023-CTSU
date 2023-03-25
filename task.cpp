//
// Created by Centauria V. CHEN on 2023/3/22.
//
#include "task.h"
#include "collision.h"
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
        do {
            task = getPendingTask(robot->id, robots, workbenches);
            if (task.wpo_from == Point{0, 0} || task.wpo_to == Point{0, 0})
                break;
            int16_t robot_id = checkRobotTaskTail(task.wpo_from, robots);
            if (!pending_task_list.empty() && robot_id != -1 && time_remain > 40)
            {
                task.status = PENDING;
                task.robot_id = robot_id;
                robots[robot_id]->add_target(task.wpo_from);
                robots[robot_id]->add_target(task.wpo_to);
            } else
            {
                task.status = STARTING;
                task.robot_id = robot->id;
                robot->add_target(task.wpo_from);
                robot->add_target(task.wpo_to);
            }
            task_list.push_back(task);
            item_occur_cnt[workbenches[task.wid_to]->type]++;
        } while (robot->target_queue_length() == 0);
    }
}

Task TaskManager::getPendingTask(int robot_id, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    // TODO: 让robot匹配到合适的Task
    double lowest_cost = 9999999999;
    Task best_task;
    for (auto &task: pending_task_list)
    {
        Vector2D dist = task.wpo_from - robots[robot_id]->position;                    // 计算机器人到任务领取处的距离
        double cost = task.dist + dist.norm() / (task.profit / profit[1]);             // 计算总距离 除与 获得利润系数
        if (workbenches[task.wid_to]->product_frames_remained != -1) cost += 17;       // 如果已经在加工了就把优先级往后推，换言之，优先填充不在加工的工作台
        if (workbenches[task.wid_to]->material_status == 0) cost += 11;                // 如果Demand工作台啥材料都没有就放放，等之后再给他喂材料
        if (4 <= workbenches[task.wid_to]->type && workbenches[task.wid_to]->type <= 6)// 让4，5，6保持持平状态（图二）（图四），这样可以防止4号工作台太远就没人去填充的窘状
        {
            int avg = (item_occur_cnt[4] + item_occur_cnt[5] + item_occur_cnt[6]) / 3;
            cost += 4 * (item_occur_cnt[workbenches[task.wid_to]->type] - avg);
        }
        if (workbenches[task.wid_to]->type == 9) cost += 6;
        std::vector<Point> ETCT;// Estimated Time to Complete the Task
        cost += calculateCollisionPosibility(task);
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
            Task task{s.workbench_id, d.workbench_id, PENDING, adj_matrix[s.workbench_id][d.workbench_id], 0, profit[s.item_type], -1, s.item_type, workbenches[s.workbench_id]->coordinate, workbenches[d.workbench_id]->coordinate};
            pending_task_list.push_back(task);
        }
    }
    freeSupplyDemandList();
}

void TaskManager::refreshSupply(const std::vector<std::unique_ptr<WorkBench>> &workbenches)
{
    std::set<int16_t> dedup;
    for (auto &t: task_list)
        if (t.status == STARTING || t.status == PENDING) dedup.insert(t.wid_from);
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
    for (auto &i: dedup)
        i.clear();
}

void TaskManager::freeSupplyDemandList()
{
    for (auto &i: demand_list)
        i.clear();
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

double TaskManager::calculateCollisionPosibility(Task task, int robot_id, const std::vector<std::unique_ptr<Robot>> &robots) const
{
    const double current_time = 180.0 - time_remain;
    Point dist = task.wpo_from - robots[robot_id]->position;
    double arrival_time = dist.norm() / 6 + current_time;
    Trace robot_from{robots[robot_id]->position, current_time, task.wpo_from, arrival_time};
    dist = task.wpo_to - task.wpo_from;
    Trace from_to{task.wpo_from, arrival_time, task.wpo_to, dist.norm() / 6 + arrival_time};

    std::vector<Trace> traces;
    for (auto &robot: robots)
    {
        auto targets = robot->get_targets();
        if (targets.empty()) continue;
        targets.push_front(robot->position);
        double t_start = current_time;
        for (auto i = 1; i < targets.size(); i++)
        {
            double t_end = t_start + (targets[i] - targets[i - 1]).norm() / 6.0;
            traces.emplace_back(targets[i - 1], t_start, targets[i], t_end);
            t_start = t_end;
        }
    }
    if (traces.empty()) return 0.0;
    std::vector<double> probs;
    std::transform(traces.cbegin(), traces.cend(), std::back_inserter(probs), [robot_from, from_to](auto trace) {
        return std::max(robot_from * trace, from_to * trace);
    });
    return *std::max_element(probs.cbegin(), probs.cend());
}