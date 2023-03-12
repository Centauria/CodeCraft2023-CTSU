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

    std::cout<<"Read the information of frames: "<<std::endl;
    std::cout<<frameID<<"  "<<money<<"  "<<std::endl;
    std::cout<<K<<std::endl;
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
            <<product_frames_remained<<" "
            <<material_status<<"  "<<product_status<<std::endl;
    }

    for(int i = 0; i < Robots_num; i++){
        std::cin.get();
        uint8_t robot_id = i;                 // 机器人ID
        int workBench_id;       // 所处工作台id
        uint8_t item_type;          // 携带物品类型
        double time_val;      // 时间价值系数
        double collision_val; // 碰撞价值系数
        double omega;         // 角速度
        double vel_x;        // 线速度 x
        double vel_y;               //线速度 y
        double orientation;// 朝向
        double coord_x;             // 坐标类 x
        double coord_y;             //坐标类 y

        std::cin>>workBench_id>>item_type>>time_val>>collision_val
            >>omega>>vel_x>>vel_y>>orientation>>coord_x>>coord_y;

        workBench_id = workBench_id;
        std::cout<<workBench_id<<"  "<<item_type<<"  "<<time_val<<"  "
            <<collision_val<<"  "<<omega<<"  "<<vel_x<<"  "<<vel_y
            << "  "<<orientation<<"  "<<coord_x<<"  "<<coord_y<<std::endl;
        Robot r(robot_id, workBench_id, item_type, time_val, collision_val, omega, 
            vel_x,vel_y, orientation, coord_x, coord_y);
        robots.emplace_back(r);
    }
    std::cout<<workbenches.size()<<"  "<<robots.size()<<std::endl;


    return true;
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
}
void Center::decide()
{
    // TODO: Set target for every robot
    for (auto &r: robots)
    {
        r.set_target(Point{10, 10});
    }
}
