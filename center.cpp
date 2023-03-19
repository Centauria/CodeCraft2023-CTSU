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


Center::Center()
{
    robots = std::vector<std::unique_ptr<Robot>>();
    workbenches = std::vector<std::unique_ptr<WorkBench>>();
    for (int i = 0; i < 4; i++)
    {
        robots_goal[i].giver_id = -10;
        robots_goal[i].receiver_id = -10;
        robots_goal[i].item_type = -10;
    }
    memset(item_occur_cnt, 0, sizeof(item_occur_cnt));
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
    while (fgets(line, sizeof line, stdin) && i < maps_row_num)
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // 判断每一行的信息
        for (int j = 0; j < maps_col_num; j++)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
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

    std::cout << "OK" << std::endl;
    std::flush(std::cout);
}
bool Center::refresh()
{
    int frameID;
    std::cin >> frameID;
    if (frameID == EOF) return false;
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
        std::cin >> robot->workbench_id >> robot->item_type >> robot->time_val >> robot->collision_val >> robot->omega >> robot->velocity.x >> robot->velocity.y >> robot->orientation >> robot->coordinate.x >> robot->coordinate.y;
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
        robot->step(deltaFrame / frameRate);
        if (robot->item_type >= 1)
        {
            if (robot->workbench_id == robots_goal[robot->id].receiver_id)
            {
                robot->sell();
                item_occur_cnt[robot->item_type]++;
                robots_goal[robot->id].receiver_id = -10;
                robots_goal[robot->id].item_type = -10;
            }
        } else
        {
            if (robot->workbench_id == robots_goal[robot->id].giver_id)
            {
                robot->buy();
                robots_goal[robot->id].giver_id = -10;
            }
        }
    }
    std::cout << "OK" << std::endl;
    LOG("logs/behavior.log", "OK")
    std::flush(std::cout);
}
void Center::decide()
{
    // TODO: Set target for every robot
    // By adjusting the variable "target" I can control the movement of the robot
    // Since "target" is a private variable
    // I should call "set_target" to change its value!
    for (auto &robot: robots)
    {
        if (tasklist.empty())
        {
            // std::cerr << "tasklist is empty!" << std::endl;
            return;
        }
        if (robots_goal[robot->id].item_type <= 0)
        {
            robots_goal[robot->id] = tasklist.front();
            tasklist.pop();
            // std::cerr << robot->id << " gets task!" << std::endl;
        }
        if (robot->item_type >= 1)
        {
            robot->set_target(robots_goal[robot->id].receiver_point);
        } else
        {
            robot->set_target(robots_goal[robot->id].giver_point);
        }
        std::vector<Point> obstacles;
        for (auto &r: robots)
        {
            if (r != robot)
            {
                obstacles.emplace_back(r->coordinate);
            }
        }
        robot->set_obstacle(obstacles);
    }
    return;
}

void Center::UpdateSupply()
{
    std::set<int> supply_check;
    // 用set来记录robot在做的任务给予者位置，避免重复领取item
    for (int i = 0; i < 4; i++)
        supply_check.insert(robots_goal[i].giver_id);

    for (int t = 7; t >= 1; t--)
    {
        for (auto &workbench: workbenches)
        {
            if (!workbench->product_status)
                continue;
            if (supply_check.count(workbench->id))
                continue;
            if (workbench->type != t)
                continue;
            Supply temp;
            temp.workbench_id = workbench->id;
            temp.workbrench_point.x = workbench->coordinate.x;
            temp.workbrench_point.y = workbench->coordinate.y;
            temp.workbrench_type = workbench->type;
            temp.item_type = workbench->type;
            supply_list[t].push(temp);
        }
    }
    supply_check.clear();
    return;
}

void Center::UpdateDemand()
{
    std::set<int> demand_check[15];
    for (int i = 0; i < 4; i++)
        demand_check[abs(robots_goal[i].item_type)].insert(robots_goal[i].receiver_id);
    for (int t = 7; t >= 1; t--)
    {
        for (auto &workbench: workbenches)
        {
            if (workbench->isFree(t))
                continue;
            if (demand_check[t].count(workbench->id))
                continue;
            if (!workbench->needRawMaterial(t))
                continue;
            Demand temp;
            temp.workbench_id = workbench->id;
            temp.workbrench_point.x = workbench->coordinate.x;
            temp.workbrench_point.y = workbench->coordinate.y;
            temp.workbrench_type = workbench->type;
            temp.item_type = t;
            demand_list[t].push(temp);
        }
    }
    for (int i = 0; i < 10; i++)
        demand_check[i].clear();
    return;
}

void Center::set_TaskingOrder()
{
    std::set<int> flag;
    for (int i = 1; i <= 7; i++)
    {
        int min_val = 99999999;
        int item_index = 1;
        for (int j = 1; j <= 7; j++)
        {
            if (flag.count(j))
                continue;
            if (min_val >= item_occur_cnt[j])
            {
                min_val = item_occur_cnt[j];
                item_index = j;
            }
        }
        TaskingOrder.push(item_index);
        flag.insert(item_index);
    }
    flag.clear();
}

void Center::UpdateTask()
{
    UpdateSupply();
    UpdateDemand();
    set_TaskingOrder();
    while (TaskingOrder.size())
    {
        int t = TaskingOrder.front();
        TaskingOrder.pop();
        while (supply_list[t].size() && demand_list[t].size())
        {
            Demand d = demand_list[t].front();
            Supply s = supply_list[t].front();
            demand_list[t].pop();
            supply_list[t].pop();
            Task temp;
            temp.item_type = t;
            //--------------------giver----------
            temp.giver_id = s.workbench_id;
            temp.giver_type = s.workbrench_type;
            temp.giver_point.x = s.workbrench_point.x;
            temp.giver_point.y = s.workbrench_point.y;
            // ------------------receiver--------
            temp.receiver_id = d.workbench_id;
            temp.receiver_type = d.workbrench_type;
            temp.receiver_point.x = d.workbrench_point.x;
            temp.receiver_point.y = d.workbrench_point.y;
            tasklist.push(temp);
        }
    }
    return;
}

void Center::FreeTaskList()
{
    std::queue<Task>().swap(tasklist);
    if (tasklist.size())
        std::cerr << "------------------------error-------------------" << std::endl;
    for (int i = 7; i >= 1; i--)
    {
        while (supply_list[i].size())
        {
            supply_list[i].pop();
        }
        if (supply_list[i].size())
            std::cerr << "------------------------error-------------------" << std::endl;
    }
    for (int i = 7; i >= 1; i--)
    {
        while (demand_list[i].size())
        {
            demand_list[i].pop();
        }
        if (demand_list[i].size())
            std::cerr << "------------------------error-------------------" << std::endl;
    }
    return;
}