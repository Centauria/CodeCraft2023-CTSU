//
// Created by Centauria V. CHEN on 2023/3/22.
//
#include <queue>
#include <vector>
#include "task.h"
TaskManager::TaskManager(size_t n_workbenches, size_t n_robots)
{
}

void TaskManager::distributeTask(std::vector<std::unique_ptr<Robot>> robots, std::vector<std::unique_ptr<WorkBench>> workbenches){
    for(const auto &robot: robots){
        if(!robot->target_queue_length()){// EXPLAIN：如果robot还有任务在身就不接任务。
            continue;
        }
        Task task = getPendingTask(robot->id);
        task_list.push_back(task);
        robot->add_target(workbenches[task.wid_from]->coordinate);
        robot->add_target(workbenches[task.wid_to]->coordinate);
        removeRelatedTask(task);
    }
}

Task TaskManager::getPendingTask(int robot_id)
{
    // TODO: 让所有robot匹配到合适的Task，并对task_list做出相应更新
    return {};
}
void TaskManager::refreshPendingTask(std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // TODO: 根据所有工作台状态，刷新pending_task_list
    std::queue<Supply> supply_list;
    std::vector<Demand> demand_list[10];
    refreshSupply(workbenches);
    refreshDemand(workbenches);
    while (supply_list.size())
    {
        Supply s = supply_list.front();
        supply_list.pop();
        if(demand_list[s.item_type].empty())
            continue;
        for (auto d: demand_list[s.item_type])
        {
            Task task;
            task.wid_from = s.workbench_id;
            task.wid_to = d.workbench_id;
            task.status = STARTING;
            // TODO: Define task
            pending_task_list.push_back(task);
        }
    }
}

void TaskManager::refreshSupply(std::vector<std::unique_ptr<WorkBench>> workbenches){
    for(auto &w: workbenches){
        if(w->isReady()){
            Supply s;
            s.item_type = w->type;
            s.workbench_id = w->id;
            supply_list.push(s);
        }
    }
}

void TaskManager::refreshDemand(std::vector<std::unique_ptr<WorkBench>> workbenches){

}

void TaskManager::removeRelatedTask(Task task){

}
