//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#include "center.h"
#include "point.h"
#include "task.h"


Center::Center()
{
    robots = std::vector<std::unique_ptr<Robot>>();
    workbenches = std::vector<std::unique_ptr<WorkBench>>();
}

void Center::initialize()
{
    char line[1024];
    int16_t robot_num = 0;
    const int maps_row_num = 100;
    const int maps_col_num = 100;
    int i = 0;
    // 从地图的左上角开始， i 表示行数，j表示列数
    // 假定地图数据为100*100的
    std::vector<Point> workbench_position;
    // sorry 我的直觉告诉我，上面这两个vector 应该没有必要声明。你应该已经写过了，就是底下那个东西的，但我不太会用，所以就自己又写了一个
    while (fgets(line, sizeof line, stdin) && i < maps_row_num)
    {
        if (line[0] == 'O' && line[1] == 'K') break;
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
                robots.emplace_back(std::make_unique<Robot>(Robot{robot_num, 0.25 + 0.5 * j, 49.75 - 0.5 * i}));
                robots.back()->_logging_name = "robot_" + std::to_string(robot_num);
                robot_num++;
            }
        }
        i++;
    }
    taskmanager.set_adj_matrix(workbench_position);
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
//    count_max_money(money);
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
        taskmanager.refreshTaskStatus(robot->id, action, workbench_point, workbenches);
    }
    taskmanager.clearOverTask();
    taskmanager.clearPendingTaskList();
    std::cout << "OK" << std::endl;
    LOG("logs/behavior.log", "OK")
    std::flush(std::cout);
}
void Center::decide()
{
    // TODO: Set target for every robot
    // By add target to queue<target> I can control the movement of the robot
    taskmanager.refreshPendingTask(workbenches);
    taskmanager.distributeTask(robots, workbenches);
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

void Center::count_max_money(int money)
{
    max_money = std::max(max_money, money);
    if (currentFrame == 9000)
    {
        std::cerr << std::endl
                  << max_money << std::endl;
    }
}