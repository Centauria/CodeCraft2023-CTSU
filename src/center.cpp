//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>
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

    for (auto &workbench: workbenches)
    {
        std::cin.get();
        std::cin >> workbench.type >> workbench.coordinate.x >> workbench.coordinate.y >> workbench.product_frames_remained >> workbench.material_status >> workbench.product_status;
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
    Point p{25, 8}, q{25, 45};
    for (auto &r: robots)
    {
        if ((r.coordinate - p).norm() < 1)
        {
            r.set_target(q);
        } else if ((r.coordinate - q).norm() < 1)
        {
            r.set_target(p);
        } else if ((r.coordinate - Point{0, 0}).norm() < 16)
        {
            r.set_target(p);
        }
    }
}
