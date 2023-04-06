//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#include "center.h"
#include "path.h"
#include "point.h"
#include "system.h"
#include "task.h"


Center::Center()
{
    robots = std::vector<std::shared_ptr<Robot>>();
    workbenches = std::vector<std::shared_ptr<WorkBench>>();
}

void Center::initialize()
{
    std::string line;
    int16_t robot_num = 0;
    int i = 0;
    // 从地图的左上角开始， i 表示行数，j表示列数
    // 假定地图数据为100*100的
    std::vector<Point> workbench_position;
    while (true)
    {
        std::cin >> std::ws >> line;
        if (std::cin.eof() || line == "OK") break;
        map.append_line(line);
        // 判断每一行的信息
        for (int j = 0; j < maps_col_num; j++)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
                workbench_position.emplace_back(0.25 + 0.5 * j, 49.75 - 0.5 * i);
                auto type = int16_t(line[j] - '0');
                workbenches.emplace_back(std::make_unique<WorkBench>(WorkBench{type, 0.25 + 0.5 * j, 49.75 - 0.5 * i}));
            } else if (line[j] == 'A')
            {
                // TODO: Cannot read robot before first workbench
                robots.emplace_back(std::make_unique<Robot>(Robot{robot_num, 0.25 + 0.5 * j, 49.75 - 0.5 * i}));
                robots.back()->_logging_name = "robot_" + std::to_string(robot_num);
                robot_num++;
            }
        }
        i++;
    }
    std::vector<System> systems = get_systems(map, robots, workbenches);
    for (auto &r: robots)
    {
        r->set_map(map);
    }
    task_manager = std::make_unique<TaskManager>();
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
    workbench_position.clear();
}
bool Center::refresh()
{
    int frameID;
    std::cin >> frameID;
    if (std::cin.eof()) return false;
    deltaFrame = frameID - currentFrame;
    currentFrame = frameID;
    int money;
    std::cin >> money;
    std::cin.get();
    int workbench_count;
    std::cin >> workbench_count;
    assert(workbench_count == workbenches.size());
    int16_t workbench_id_counter = 0;
    for (auto &workbench: workbenches)
    {
        std::cin.get();
        std::cin >> workbench->type >> workbench->coordinate.x >> workbench->coordinate.y >> workbench->product_frames_remained >> workbench->material_status >> workbench->product_status;
        workbench->id = workbench_id_counter;
        workbench_id_counter++;
    }

    for (auto &robot: robots)
    {
        std::cin.get();
        std::cin >> robot->workbench_id >> robot->item_type >> robot->time_val >> robot->collision_val >> robot->omega >> robot->velocity.x >> robot->velocity.y >> robot->orientation >> robot->position.x >> robot->position.y;
    }
    std::string ok;
    std::cin >> ok;
    assert(ok == "OK");

    return true;
}
void Center::step()
{
    std::cout << currentFrame << std::endl;
    for (auto &robot: robots)
    {
        const auto [action, workbench_point] = robot->step(deltaFrame / frameRate);
        task_manager->refreshTaskStatus(action, robot->id);
    }
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
}
void Center::decide()
{
    task_manager->refreshSupply(workbenches);
    task_manager->refreshDemand(workbenches);
    if(task_manager->haveTask()){
        for (auto &robot: robots)
        {
            if (!robot->target_queue_length())
            {//机器人无任务
                //分配任务
                if(robot->id == 2){

                }
                task_manager->distributeTask(robot->id, robots, workbenches, map);
            }
        }
    }
    task_manager->freeSupplyDemandList();
    for (auto &robot: robots)
    {
        std::vector<std::unique_ptr<Object>> obstacles;
        for (auto &r: robots)
        {
            if (r != robot)
            {
                obstacles.emplace_back(std::make_unique<Object>(dynamic_cast<Object &>(*r)));
            }
        }
        robot->set_obstacle(obstacles);
    }
}
