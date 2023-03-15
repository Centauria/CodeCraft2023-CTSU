//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>
#include <set>
#include <vector>

#include "center.h"
#include "math/point.h"


Center::Center()
{
    robots = std::vector<Robot>();
    workbenches = std::vector<WorkBench>();
}
void Center::initialize()
{
    char line[1024];
    uint8_t robot_num = 0;
    const int maps_row_num = 100;
    const int maps_col_num = 100;
    int i = 0;
    // 从地图的左上角开始， i 表示行数，j表示列数
    // 地图数据为100*100的
    while (fgets(line, sizeof line, stdin) && i < maps_row_num)
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // 判断每一行的信息
        for (int j = 0; j < maps_col_num; j++)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
                // 读到的字符代表着工作台
                int type = int(line[j] - '0');
                workbenches.emplace_back(type, 0.25 + 0.5 * j, 49.75 - 0.5 * i);

            } else if (line[j] == 'A')
            {
                // 读到的字符代表着机器人
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
    int workbench_id_counter = 0;
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
    }
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
}
void Center::decide()
{
    // TODO: Set target for every robot
    // By adjusting the variable "target" I can control the movement of the robot
    // Since "target" is a private variable
    // I should call "set_target" to change its value!
    Point p{25, 8}, q{25, 45};
    for (auto &r: robots)
    {
        if ((r.coordinate - p).norm() < 1)
        {
            r.set_target(q);
        } else if ((r.coordinate - q).norm() < 1)
        {
            r.set_target(p);
        } else if (r.coordinate.norm() < 16)
        {
            r.set_target(p);
        }
    }
}

void Center::UpdateSupply(std::queue<Supply> (&supply_list)[10])
{
    std::set<int> robot_tasking_supply_id;
    // 用set来记录robot在做的任务给予者位置，避免重复领取item
    for (int i = 0; i < 4; i++)
        robot_tasking_supply_id.insert(robots_goal[i].giver_id);

    for (int t = 7; t >= 1; t--)
    {
        for (auto workbrench: workbenches)
        {
            if (!workbrench.product_status)
                continue;
            if (robot_tasking_supply_id.count(workbrench.id))
                continue;
            if (workbrench.type != t)
                continue;
            Supply temp;
            temp.workbench_id = workbrench.id;
            temp.workbrench_point = workbrench.coordinate;
            temp.workbrench_type = workbrench.type;
            temp.item = workbrench.type;
            supply_list[t].push(temp);
        }
    }
    return;
}

void Center::UpdateDemand(std::queue<Demand> (&demand_list)[10])
{
    std::set<int> robot_tasking_demand_id;
    for (int i = 0; i < 4; i++)
        robot_tasking_demand_id.insert(robots_goal[i].receiver_id);
    for (int t = 7; t >= 1; t--)
    {
        for (auto workbrench: workbenches)
        {
            if (workbrench.isFree(t))
                continue;
            if (robot_tasking_demand_id.count(workbrench.id))
                continue;
            if (!workbrench.needRawMaterial(t))
                continue;
            Demand temp;
            temp.workbench_id = workbrench.id;
            temp.workbrench_point = workbrench.coordinate;
            temp.workbrench_type = workbrench.type;
            temp.item = t;
            demand_list[t].push(temp);
        }
    }
    return;
}

void Center::UpdateTask()
{
    std::queue<Supply> supply_list[10];
    std::queue<Demand> demand_list[10];
    UpdateSupply(supply_list);
    UpdateDemand(demand_list);
    for (int t = 7; t >= 1; t--)
    {
        while (supply_list[t].size() && demand_list[t].size())
        {
            Demand d = demand_list[t].front();
            Supply s = supply_list[t].front();
            demand_list[t].pop();
            supply_list[t].pop();
            Task temp;
            temp.item = t;
            //--------------------giver----------
            temp.giver_id = s.workbench_id;
            temp.giver_type = s.workbrench_type;
            temp.giver_point = s.workbrench_point;
            // ------------------receiver--------
            temp.receiver_id = d.workbench_id;
            temp.receiver_type = d.workbrench_type;
            temp.receiver_point = d.workbrench_point;
            tasklist.push(temp);
        }
    }
    return;
}