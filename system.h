//
// Created by 蔡恩光 on 2023/4/8.
//

#ifndef CODECRAFTSDK_SYSTEM_H
#define CODECRAFTSDK_SYSTEM_H
#include "concept.h"
#include <unordered_set>

class System
{
public:
    //获取当前system里面的所有workbench和robot
    //需要有一个就能够判断胖子能否从一个工作台到另一个工作台的function

    std::vector<size_t> w_ids;
    std::vector<size_t> r_ids;

    void refresh();
    void decide();
    Task getPendingTask(int robot_id);

    void set_shared(std::vector<WorkBench> &w, std::vector<Robot> &r);
    void build_cache(CMatrix &map);
    double calculateCost(const Task &task, int robot_id);
    void refreshSupply();
    void refreshDemand();
    void refreshPendingTask();
    void clear();

private:
    std::list<Task> pending_task_list;
    std::vector<Path> path_cache;
    std::vector<double> cost_cache;
    DMatrix cost_index_map{1, 1};
    std::vector<SD> supply_list;
    std::vector<SD> demand_list[10];
    std::vector<WorkBench> *workbenches;
    std::vector<Robot> *robots;
};

System flood_fill(CMatrix &map, bool visited[][100], Index start, std::unordered_set<Index, HashFunction> &workbench_set, std::unordered_set<Index, HashFunction> &robot_set, std::vector<WorkBench> &workbenches, std::vector<Robot> &robots);
std::vector<System> get_system(CMatrix &map, std::vector<WorkBench> &workbenches, std::vector<Robot> &robots);

#endif//CODECRAFTSDK_SYSTEM_H