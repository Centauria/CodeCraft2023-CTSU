#include <chrono>
#include <iostream>
#include <thread>

#include "io.h"
#include "system.h"
using namespace std;

int main()
{
    // Uncomment this to start debugging
    // std::this_thread::sleep_for(10s);
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    setbuf(stdout, nullptr);
    auto map_string = input_map();
    auto map = read_map(map_string);
    auto workbenches = read_workbenches(map_string);
    auto robots = read_robots(map_string);
    cout << "OK" << endl;
    Timer timer;
    auto systems = get_system(map, workbenches, robots);
    while (input_frame(workbenches, robots, timer))
    {
        cout << timer.current_frame << endl;
        for (auto &system: systems)
        {
            system.refresh(robots);
            for (auto i: system.r_ids)
            {
                auto robot = robots[i];
                robot.step(timer.delta, map);
            }
        }
    }
    return 0;
}
