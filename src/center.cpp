//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>
#include <vector>

#include "center.h"


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
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // TODO: Initialize
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
