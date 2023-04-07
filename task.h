//
// Created by 蔡恩光 on 2023/4/5.
//

#ifndef CODECRAFTSDK_TASK_H
#define CODECRAFTSDK_TASK_H

#include "workbench.h"
#include "robot.h"
#include "path.h"
#include <list>

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
    double aggregated_index = 0;
    int16_t robot_id = -1;
    int16_t item_type = 0;
    Point wpo_from;
    Point wpo_to;
    TaskStatus status = PENDING;
};

const int profit[]{
        0,
        3000,
        3200,
        3400,
        7100,
        7800,
        8300,
        29000
};


struct SD {
    int16_t workbench_id;
    int16_t workbench_type;
    int16_t item_type;
};

class TaskManager
{
public:
    void distributeTask(int16_t robot_id, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches, GameMap &map);
    void refreshSupply(const std::vector<std::shared_ptr<WorkBench>> &workbenches);
    void refreshDemand(const std::vector<std::shared_ptr<WorkBench>> &workbenches);
    double calculateAggregatedIndex(Path &a, Path&b, double gross_income);
    Path getPath(Point &a, Point &b, const std::vector<std::shared_ptr<WorkBench>> &workbenches, GameMap &map, int width);
    void refreshTaskStatus(Trade action, int16_t robot_id);
    void freeSupplyDemandList();
    bool haveTask();
//    void set_RobotWokbench_Belonging(GameMap map, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches);

private:
    std::vector<int> workbench_belonging;
    std::vector<int> robot_belonging;
    Path path_between_workbenches[50][50];
    std::list<SD> supply_list;
    std::list<SD> demand_list[10];
    Task robotTask[4];
};


#endif//CODECRAFTSDK_TASK_H
