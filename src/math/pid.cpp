//
// Created by Centauria V. CHEN on 2023/3/12.
//

#include "pid.h"
#include <cstddef>
#include <cstdlib>
#include <ctime>
PIDController::PIDController() = default;
PIDController::PIDController(double p, double i, double d)
{
    Kp = p;
    Ki = i;
    Kd = d;
}
double PIDController::feed(double e, double dt)
{
    record(e, dt);
    return Kp * e + Ki * integral() + Kd * derivative();
}
double PIDController::feed_if(double e, double dt)
{
    return Kp * e + Ki * (integral() + e * dt) + Kd * (e - memory.back().e) / dt;
}
double PIDController::integral() const
{
    return integral_value;
}
double PIDController::derivative() const
{
    size_t size = memory.size();
    if (size > 1)
    {
        auto et = memory.back();
        auto et_1 = memory[size - 2];
        return (et.e - et_1.e) / et.dt;
    }
    return 0;
}
void PIDController::record(double e, double dt)
{
    if (refresh_count == refresh_freq)
    {
        while (memory.size() >= memory_limit)
        {
            memory.pop_front();
        }
        memory.emplace_back(ET{e, dt});
        integral_value = 0;
        for (auto et: memory)
        {
            integral_value += et.e * et.dt;
        }
        refresh_count = 0;
    } else
    {
        while (memory.size() >= memory_limit)
        {
            auto et = memory.front();
            integral_value -= et.e * et.dt;
            memory.pop_front();
        }
        memory.emplace_back(ET{e, dt});
        integral_value += e * dt;
        refresh_count++;
    }
}
