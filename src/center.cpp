//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>
#include <vector>

#include "center.h"
#include "math/point.h"


Center::Center()
{

}
void Center::initialize()
{
    char line[1024];
    int K = 0;// 记录工作台数量
    int robot_num = 0;
    std::cout << " start analysis map data " << std::endl;
    const int maps_row_num = 100;
    const int maps_col_num = 100;
    int i = 0;// 从地图的左上角开始， i 表示行数，j表示列数
    // 地图数据为100*100的
    while (fgets(line, sizeof line, stdin) && i<maps_row_num)
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // TODO: Initialize
        // std::cout<<"start get line"<<std::endl;

        // std::cout<<line<<std::endl;
        // std::cout<<i << " " <<sizeof line<<std::endl;
        // std::cout<<line<<std::endl;
        int j = 0;
        // 判断每一行的信息
        while (j<maps_col_num) 
        {
            // std::cout<< line[j]<<std::endl;
            if (line[j] >= '1' && line[j] <= '9')
            {
            // 读到的字符代表着工作台
                K++;
                int type = int(line[j] - '0');
                workbenches.emplace_back(type, 0.25 + 0.5 * j, 49.75 - 0.5 * i);
                // std::cout<< "Workbench: "<< "type: "<< type <<"  "<< "x: " <<0.25 + 0.5 * j <<"  "
                //     <<"y:" <<49.75 - 0.5 * i<<std::endl;
                
            }
            else if (line[j] == 'A')
            {
                // 读到的字符代表着机器人
                robot_num++;
                robots.emplace_back(0.25 + 0.5 * j, 49.75 - 0.5 * i);
                // std::cout<<"robot_num: "<<robot_num<<std::endl;
                // std::cout << "Robot: "<< 0.25 + 0.5 * j<<"  "
                //     << 49.75 - 0.5 * i<<std::endl;
                    
            }
            j++;
        }
        i++;
    }
    
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
    std::cout<<"K: "<<K<<std::endl;
    std::cout<<"robot_num: "<<robot_num<<std::endl;
    // std::cout<<workbenches.size()<< "  " <<robots.size()<<"  "<<std::endl;
}
bool Center::refresh()
{
    int frameID;
    std::cin >> frameID;
    if (frameID == EOF) return false;
    deltaFrame = frameID - currentFrame;
    currentFrame = frameID;
    char line[1024];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // TODO: Refresh status
    }
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
    currentFrame++;
}
