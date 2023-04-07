#include <chrono>
#include <iostream>
#include <thread>

#include "io.h"
using namespace std;

int main()
{
    // Uncomment this to start debugging
    //        std::this_thread::sleep_for(10s);
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    setbuf(stdout, nullptr);
    auto map_string = input_map();
    auto map = read_map(map_string);
    auto workbenches = read_workbenches(map_string);
    auto robots = read_robots(map_string);
    cout << "OK" << endl;
    Timer timer;
    while (input_frame(workbenches, robots, timer))
    {
        cout << timer.current_frame << endl;
        cout << "forward 1 6" << endl;
        cout << "rotate 1 3" << endl;
        cout << "OK" << endl;
    }
    return 0;
}
