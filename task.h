//
// Created by Centauria V. CHEN on 2023/3/22.
//

#ifndef CODECRAFTSDK_TASK_H
#define CODECRAFTSDK_TASK_H

#include "point.h"
#include "robot.h"
#include "workbench.h"
#include <cstdint>
#include <list>
#include <queue>
#include <vector>

enum TaskStatus
{
    PENDING,
    STARTING,
    PROCESSING,
    OVER
};

struct Task {
    int16_t wid_from = -1;
    int16_t wid_to = -1;
    TaskStatus status = PENDING;
    double dist = 9999999999;
    double cost = 9999999999;
    double profit = 0;
    int16_t robot_id = -1;
    int16_t item_type = 0;
};

struct SD {
    int16_t workbench_id;
    int16_t workbrench_type;
    //------------
    int16_t item_type;
};

class TaskManager
{
public:
    TaskManager();

    void distributeTask(const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches);// 这个函数负责检查哪些机器人目前处于无任务状态，然后给他们分发任务通过调用getPendingTask。

    Task getPendingTask(int robot_id, const std::vector<std::unique_ptr<Robot>> &robots, const std::vector<std::unique_ptr<WorkBench>> &workbenches);

    void refreshPendingTask(const std::vector<std::unique_ptr<WorkBench>> &workbenches);

    void refreshSupply(const std::vector<std::unique_ptr<WorkBench>> &workbenches);
    void refreshDemand(const std::vector<std::unique_ptr<WorkBench>> &workbenches);
    void freeSupplyDemandList();

    bool conflict(const Task &pendingtask, Task task);

    void refreshTaskStatus(Trade action, Point workbench_point, const std::vector<std::unique_ptr<WorkBench>> &workbenches);

    void clearPendingTaskList();
    void clearOverTask();
    // 此处可以写一些用workbench_id或一些其他附加条件筛选pending_task_list的函数


private:
    std::list<Task> task_list;        // 当前正在执行的任务
    std::list<Task> pending_task_list;// 处理所有潜在任务
    std::queue<SD> supply_list;
    std::vector<SD> demand_list[10];
};

#endif//CODECRAFTSDK_TASK_H
