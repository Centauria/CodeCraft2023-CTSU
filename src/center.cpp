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
    for (int i = 0; i < 4; ++i)
    {
        robots.emplace_back(i);
    }
}
void Center::initialize()
{
    char line[1024];
    const double row_interval = 0.25;
    const double col_interval = 0.25;
    int K = 0;          // 记录工作台数量
    std::cout<<" start analysis map data "<<std::endl;
    int i = 0, j = 0;       // 从地图的左上角开始， i 表示行数，j表示列数
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // TODO: Initialize
        // std::cout<<line<<std::endl;
        // 判断每一行的信息
        while(line[j] == '.'){
            j++;
        }
        
        if(line[j]>='1' && line[j]<='9'){
            // 读到的字符代表着工作台
            K++;
            int type = line[j] - '0';
            Vector2D coordinate;
            double tem_x = 0.25 + 0.5 * j;
            double tem_y = 49.75 - 0.5 * i;
            coordinate.set_x(tem_x);
            coordinate.set_y(tem_y);
            
        }
        
        if(line[j]=='A'){
            // 读到的字符代表着机器人
        }
        
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


void WrokBench::setType(int type){
    this->type = type;
}

int WrokBench::getType(){
    return type;
}


void WrokBench::setCoordinate(Vector2D coordinate){
    double x = coordinate.get_x();
    double y = coordinate.get_y();
    this->coordinate.set_x(x);
    this->coordinate.set_y(y); 
}


Vector2D WrokBench::getCoordinate(){
    return coordinate;
}


void WrokBench::setMaterials(int materials){
    this->materials = materials;
}

int WrokBench::getMaterials(){
    return materials;
}
void WrokBench::setProduct_status(bool product_status){
    this->product_status = product_status;
}

bool WrokBench::getProduct_status(){
    return product_status;
}