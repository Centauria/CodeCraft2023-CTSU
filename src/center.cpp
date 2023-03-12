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
        // TODO: Initialize
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
    fflush(stdout);
    //  清空Center类中的 workbenches, robots;
    workbenches.clear();
    robots.clear();
    const int Robots_num = 4;
    int frameID;
    std::cin >> frameID;
    if (frameID == EOF) return false;
    deltaFrame = frameID - currentFrame;
    currentFrame = frameID;
    int money;
    std::cin>>money;
    std::cin.get();
    int K;
    std::cin>>K;
    for(int i = 0; i < K; i++){
        std::cin.get();
        uint8_t type;
        double coord_x, coord_y;
        int product_frames_remained;
        uint8_t material_status;    // 原材料格状态
        bool product_status;        // 产品格状态
        std::cin>>type>>coord_x>>coord_y
            >>product_frames_remained>>material_status>>product_status;
        WorkBench w(type, coord_x, coord_y, product_frames_remained,
            material_status, product_status);
        workbenches.emplace_back(w);
        std::cout<<type<<"  "<<coord_x<<"  "<<coord_y<<"  "
            <<product_frames_remained<<" "<<material_status<<"  "<<product_status;
    }

    for(int i = 0; i < Robots_num; i++){
        std::cin.get();
        uint8_t item_type;          // 携带物品类型
        double time_val = 0.0;      // 时间价值系数
        double collision_val = 0.0; // 碰撞价值系数
        double omega = 0.0;         // 角速度
        Velocity velocity{};        // 线速度
        double orientation = M_PI_2;// 朝向
        Point coordinate;           // 坐标类
    }



    fflush(stdout);
    return false;
}
void Center::step()
{
    std::cout << currentFrame << std::endl;
    for (auto robot: robots)
    {
        robot.step();
    }
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
    currentFrame++;
}
void Center::decide()
{
    // TODO: Set target for every robot
    for (auto &r: robots)
    {
        r.set_target(Point{10, 10});
    }
}
