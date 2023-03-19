//
// Created by Centauria V. CHEN on 2023/3/18.
//

#include "delay.h"
Delay::Delay(double t)
{
    delay_time = t;
}
double Delay::feed(double e, double dt)
{
    memory.push_back(ET{e, dt});
    memory_time += dt;
    if (memory_time >= delay_time)
    {
        ET v = memory.front();
        memory.pop_front();
        memory_time -= v.dt;
        last_e = v.e;
        return v.e;
    }
    return last_e;
}
