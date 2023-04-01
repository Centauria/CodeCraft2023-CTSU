//
// Created by Centauria V. CHEN on 2023/3/29.
//

#ifndef CODECRAFTSDK_SYSTEM_H
#define CODECRAFTSDK_SYSTEM_H

#include "coordinate.h"
#include "map.h"
#include "point.h"
#include "robot.h"
#include "workbench.h"
#include <cstdint>
#include <vector>

struct Node {
    uint8_t type;
};

class Region
{
public:
    Region();
    void add_point(Index p);
    bool include(Point p);
    bool reachable(Point p, double detect_radius, double target_radius);


private:
    std::vector<Index> pts;
};

class System
{
public:
    System();

    void add_region(const Region &region);
    void add_robot(const Robot &robot);
    void add_workbench(const WorkBench &workBench);

private:
    std::vector<Region> regions;
    std::vector<std::shared_ptr<Robot>> robots;
    std::vector<std::shared_ptr<WorkBench>> workbenches;
};

void block_all_roads(DMatrix &Dmap);
System floodfill(DMatrix &Dmap, Index index, GameMap &map, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches);
std::vector<System> get_systems(GameMap map, const std::vector<std::shared_ptr<Robot>> &robots, const std::vector<std::shared_ptr<WorkBench>> &workbenches);

#endif//CODECRAFTSDK_SYSTEM_H
