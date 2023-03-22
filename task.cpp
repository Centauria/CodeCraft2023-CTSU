//
// Created by Centauria V. CHEN on 2023/3/22.
//

#include "task.h"
TaskManager::TaskManager(size_t n_workbenches, size_t n_robots)
{
}
Task TaskManager::getPendingTask(std::vector<std::unique_ptr<Robot>> robots)
{
    // TODO: 让所有robot匹配到合适的Task，并对task_list做出相应更新
    return {};
}
void TaskManager::refreshPendingTask(std::vector<std::unique_ptr<WorkBench>> workbenches)
{
    // TODO: 根据所有工作台状态，刷新pending_task_list
}
