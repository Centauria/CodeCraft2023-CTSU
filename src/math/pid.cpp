//
// Created by Centauria V. CHEN on 2023/3/12.
//

#include "pid.h"
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
double PIDController::integral()
{
    double result = 0;
    for (auto et: memory)
    {
        result += et.e * et.dt;
    }
    return result;
}
double PIDController::derivative()
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
    if (memory.size() == memory_limit)
    {
        memory.pop_front();
    }
    memory.emplace_back(ET{e, dt});
}
