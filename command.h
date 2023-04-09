//
// Created by Centauria V. CHEN on 2023/4/9.
//

#ifndef CODECRAFTSDK_COMMAND_H
#define CODECRAFTSDK_COMMAND_H

#include <iostream>

using namespace std;

inline void forward(int robot_id, double v)
{
    cout << "forward " << robot_id << " " << v << endl;
}
inline void rotate(int robot_id, double v)
{
    cout << "rotate " << robot_id << " " << v << endl;
}
inline void buy(int robot_id)
{
    cout << "buy " << robot_id << endl;
}
inline void sell(int robot_id)
{
    cout << "sell " << robot_id << endl;
}
inline void destroy(int robot_id)
{
    cout << "buy " << robot_id << endl;
}

#endif//CODECRAFTSDK_COMMAND_H
