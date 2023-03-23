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
#include "task.h"

class Center
{
public:
    Center();
    void initialize();
    bool refresh();
    void decide();
    void step();

    // ThCyber-------
    void count_max_money(int money);
    void set_adj_matrix(std::vector<Point> &workbench_position);
    TaskManager taskmanager;
    int item_occur_cnt[10];
    int adj_matrix[50][50];// distance between every two workbench

    int max_money;

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
