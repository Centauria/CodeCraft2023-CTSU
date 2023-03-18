//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "center.h"
#include "point.h"


Center::Center()
{
    robots = std::vector<Robot>();
    workbenches = std::vector<WorkBench>();
    for(int i = 0; i < 4; i++){
        robots_goal[i].giver_id = -10;
        robots_goal[i].receiver_id = -10;
        robots_goal[i].item_type = -10;
    }
}
void Center::initialize()
{
    char line[1024];
    int robot_num = 0;
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
                int type = int(line[j] - '0');
                workbenches.emplace_back(type, 0.25 + 0.5 * j, 49.75 - 0.5 * i);

            } else if (line[j] == 'A')
            {
                robots.emplace_back(robot_num, 0.25 + 0.5 * j, 49.75 - 0.5 * i);
                robots.back()._logging_name = "robot_" + std::to_string(robot_num);
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
        std::cin >> workbench.type >> workbench.coordinate.x >> workbench.coordinate.y >> workbench.product_frames_remained >> workbench.material_status >> workbench.product_status;
        workbench.id = workbench_id_counter;
        workbench_id_counter++;
    }

    for (auto &robot: robots)
    {
        std::cin.get();
        std::cin >> robot.workbench_id >> robot.item_type >> robot.time_val >> robot.collision_val >> robot.omega >> robot.velocity.x >> robot.velocity.y >> robot.orientation >> robot.coordinate.x >> robot.coordinate.y;
    }

    std::string ok;
    std::cin >> ok;
    assert(ok == "OK");

    return true;
}
void Center::step()
{
    std::cout << currentFrame << std::endl;
    for (auto robot: robots)
    {
        robot.step(deltaFrame / frameRate);
        if (robot.item_type >= 1)
        {
            if (robot.workbench_id == robots_goal[robot.id].receiver_id)
            {
                robot.sell();
                robots_goal[robot.id].receiver_id = -10;
                robots_goal[robot.id].item_type = -10;
                // std::set<int> check_receiver;
                // std::set<int> check_giver;
                // // 用check来记录robot在做的任务，避免重复任务
                // // 有了这个，我就可以给每一个Robots一个自己的为他量身定做过的 priority_queue
                // // 甚至我还可以让提前买完东西以后查看当前工作台 是否有货可以购买，如果有的话立刻购买当前工作台的货物，然后找到Demand把货送过去
                // for (int i = 0; i < 4; i++)
                // {
                //     if (i == robot.id)
                //         continue;
                //     check_giver.insert(robots_goal[robot.id].giver_id);
                //     check_receiver.insert(robots_goal[robot.id].receiver_id);
                // }
                // if (!tasklist.empty())
                // {
                //     do {
                //         robots_goal[robot.id] = tasklist.front();
                //         tasklist.pop();
                //     } while (check_giver.count(robots_goal[robot.id].giver_id) || check_receiver.count(robots_goal[robot.id].receiver_id));
                // }
                // check_giver.clear();
                // check_receiver.clear();
            }
        } else
        {
            if (robot.workbench_id == robots_goal[robot.id].giver_id)
            {
                robot.buy();
                robots_goal[robot.id].giver_id = -10;
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
    for (auto &r: robots)
    {
        if (tasklist.empty())
        {
            // std::cerr << "tasklist is empty!" << std::endl;
            return;
        }
        if (robots_goal[r.id].item_type <= 0)
        {
            robots_goal[r.id] = tasklist.front();
            tasklist.pop();
            // std::cerr << r.id << " gets task!" << std::endl;
        }
        if (r.item_type >= 1)
        {
            r.set_target(robots_goal[r.id].receiver_point);
        } else
        {
            r.set_target(robots_goal[r.id].giver_point);
        }
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
        for (auto workbrench: workbenches)
        {
            if (!workbrench.product_status)
                continue;
            if (supply_check.count(workbrench.id))
                continue;
            if (workbrench.type != t)
                continue;
            Supply temp;
            temp.workbench_id = workbrench.id;
            temp.workbrench_point.x = workbrench.coordinate.x;
            temp.workbrench_point.y = workbrench.coordinate.y;
            temp.workbrench_type = workbrench.type;
            temp.item_type = workbrench.type;
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
        for (auto workbrench: workbenches)
        {
            if (workbrench.isFree(t))
                continue;
            if (demand_check[t].count(workbrench.id))
                continue;
            if (!workbrench.needRawMaterial(t))
                continue;
            Demand temp;
            temp.workbench_id = workbrench.id;
            temp.workbrench_point.x = workbrench.coordinate.x;
            temp.workbrench_point.y = workbrench.coordinate.y;
            temp.workbrench_type = workbrench.type;
            temp.item_type = t;
            demand_list[t].push(temp);
        }
    }
    for (int i = 0; i < 10; i++)
        demand_check[i].clear();
    return;
}

void Center::UpdateTask()
{
    UpdateSupply();
    UpdateDemand();
    for (int t = 7; t >= 1; t--)
    {
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
    if(tasklist.size())
         std::cerr << "------------------------error-------------------" << std::endl;
    for (int i = 7; i >= 1; i--)
    {
        while (supply_list[i].size())
        {
            supply_list[i].pop();
        }
        if(supply_list[i].size())
             std::cerr << "------------------------error-------------------" << std::endl;
    }
    for (int i = 7; i >= 1; i--)
    {
        while (demand_list[i].size())
        {
            demand_list[i].pop();
        }
        if(demand_list[i].size())
            std::cerr << "------------------------error-------------------" << std::endl;
    }
    return;
}