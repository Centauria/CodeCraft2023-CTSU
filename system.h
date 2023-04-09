//
// Created by 蔡恩光 on 2023/4/8.
//

#ifndef CODECRAFTSDK_SYSTEM_H
#define CODECRAFTSDK_SYSTEM_H
#include "concept.h"
#include <list>
#include <unordered_set>


struct Task {
    int rid = -1;
    Path path;
    int from_id = -1, to_id = -1;
    int item_type = 0;
};

struct SD {
    int workbench_id;
    int item_type;
};

class System
{
public:
    std::vector<int> w_ids;
    std::vector<int> r_ids;
    std::list<Task> processing_task;

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