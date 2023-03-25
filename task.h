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
#include <cstring>

enum TaskStatus
{
    PENDING,
    STARTING,
    PROCESSING,
    OVER
};

struct Task {
    int16_t wid_from = 0;
    int16_t wid_to = 0;
    TaskStatus status = PENDING;
    double dist = 99999;
    double cost = 99999;
    double profit = 0;
    int16_t robot_id = -1;
    int16_t item_type = 0;
    Point wpo_from;
    Point wpo_to;
};

struct SD {
    int16_t workbench_id;
    int16_t workbench_type;
    int16_t item_type;
};

const double profit[]{
        0,
        3000,
        3200,
        3400,
        7100,
        7800,
        8300,
        29000};

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
    void refreshTaskStatus(int robot_id, Trade action, Point workbench_point, const std::vector<std::unique_ptr<WorkBench>> &workbenches);
    void clearPendingTaskList();
    void clearOverTask();
    int16_t checkRobotTaskTail(Point x, const std::vector<std::unique_ptr<Robot>> &robots);
    double calculateCollisionPosibility(Task task, int robot_id, const std::vector<std::unique_ptr<Robot>> &robots) const;
    // 此处可以写一些用workbench_id或一些其他附加条件筛选pending_task_list的函数

    double adj_matrix[50][50];// distance between every two workbench
    void set_adj_matrix(const std::vector<Point> &workbench_position);

    int item_occur_cnt[10];

    double time_remain = 180.0;
    void set_sec_remain(int currentFrame);

private:
    std::list<Task> task_list;        // 当前正在执行的任务
    std::list<Task> pending_task_list;// 处理所有潜在任务
    std::queue<SD> supply_list;
    std::vector<SD> demand_list[10];
};

#endif//CODECRAFTSDK_TASK_H
