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
    int workbench_id;
    uint8_t workbrench_type;
    //------------
    uint8_t item;
};

struct Demand {
    Point workbrench_point;
    int workbench_id;
    uint8_t workbrench_type;
    //------------
    uint8_t item;
};
struct Task {
    int item;
    int giver_type, receiver_type;
    int giver_id, receiver_id;
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
    void UpdateSupply(std::queue<Supply> (&supply_list)[10]);
    void UpdateDemand(std::queue<Demand> (&demand_list)[10]);
    void UpdateTask();

private:
    int currentFrame = 0;
    int deltaFrame = 0;// Difference between this frame and last frame
    double frameRate = 50;
    std::vector<WorkBench> workbenches;
    std::vector<Robot> robots;
};


#endif//CODECRAFTSDK_CENTER_H
