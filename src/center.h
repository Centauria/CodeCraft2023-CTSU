//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <queue>
#include <string>
#include <vector>

#include "math/point.h"
#include "robot.h"
#include "workbench.h"

struct Supply {
    Point workbrench_point;
    uint16_t workbench_id;
    uint16_t workbrench_type;
    //------------
    uint16_t item_type;
};

struct Demand {
    Point workbrench_point;
    uint16_t workbench_id;
    uint16_t workbrench_type;
    //------------
    uint16_t item_type;
};
struct Task {
    uint16_t item_type = 0;
    uint16_t giver_type = 0, receiver_type = 0;
    uint16_t giver_id = 0, receiver_id = 0;
    Point giver_point;
    Point receiver_point;
};

class Center
{
public:
    Center();
    void initialize();
    bool refresh();
    void decide();
    void step();

    // ThCyber-------
    Task robots_goal[4];
    std::queue<Task> tasklist;
    std::queue<Supply> supply_list[10];
    std::queue<Demand> demand_list[10];
    void UpdateSupply();
    void UpdateDemand();
    void UpdateTask();
    void FreeTaskList();

private:
    int currentFrame = 0;
    int deltaFrame = 0;// Difference between this frame and last frame
    double frameRate = 50;
    std::vector<WorkBench> workbenches;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
