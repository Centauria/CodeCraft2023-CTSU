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
    double value = 0;
    int16_t rid = -1;
};

class TaskManager
{
public:
    TaskManager(size_t n_workbenches, size_t n_robots);

    Task getPendingTask(std::vector<std::unique_ptr<Robot>> robots);

    void refreshPendingTask(std::vector<std::unique_ptr<WorkBench>> workbenches);

    // 此处可以写一些用workbench_id或一些其他附加条件筛选pending_task_list的函数


private:
    std::vector<Task> task_list;      // 当前正在执行的任务
    std::list<Task> pending_task_list;// 处理所有潜在任务
};

#endif//CODECRAFTSDK_TASK_H
