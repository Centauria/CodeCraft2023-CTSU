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
    std::vector<WorkBench> workbenches;
    std::vector<Robot> robots;
    //获取当前system里面的所有workbench和robot
    //需要有一个就能够判断胖子能否从一个工作台到另一个工作台的function
private:
};

System floodfill(CMatrix &map, bool visited[][100], Index start, std::unordered_set<Index, HashFunction> &workbench_set, std::unordered_set<Index, HashFunction> &robot_set, std::vector<WorkBench> &workbenches, std::vector<Robot> &robots);
std::vector<System> get_system(CMatrix map, std::vector<WorkBench> &workbenches, std::vector<Robot> &robots);

#endif//CODECRAFTSDK_SYSTEM_H