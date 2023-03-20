//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_CENTER_H
#define CODECRAFTSDK_CENTER_H

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "point.h"
#include "robot.h"
#include "workbench.h"

struct Supply {
    Point workbrench_point;
    int16_t workbench_id;
    int16_t workbrench_type;
    //------------
    int16_t item_type;
};

struct Demand {
    Point workbrench_point;
    int16_t workbench_id;
    int16_t workbrench_type;
    //------------
    int16_t item_type;
};
struct Task {
    int16_t item_type = 0;
    int16_t giver_type = 0, receiver_type = 0;
    int16_t giver_id = 0, receiver_id = 0;
    Point giver_point;
    Point receiver_point;
    bool status = false;
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
    std::queue<Supply> supply_list[10];
    std::vector<Demand> demand_list[10];
    void UpdateSupply();
    void UpdateDemand();
    bool get_Task(int robot_id);
    void FreeSupplyDemandList();
    std::queue<int> TaskingOrder;
    int item_occur_cnt[10];
    void setTaskingOrder();
    int adj_matrix[50][50];// distance between every two workbench 
    void set_adj_matrix(std::vector<Point> &workbench_position);

    // Logging 相关属性
    std::string _logging_name = "center";

private:
    int currentFrame = 0;
    int deltaFrame = 0;// Difference between this frame and last frame
    double frameRate = 50;
    std::vector<std::unique_ptr<WorkBench>> workbenches;
    std::vector<std::unique_ptr<Robot>> robots;
};


#endif//CODECRAFTSDK_CENTER_H
