#include "src/center.h"
#include <chrono>
#include <thread>
using namespace std;

int main()
{
    // Uncomment this to start debugging
    //std::this_thread::sleep_for(30s);
    Center center;
    center.initialize();
    while (center.refresh())
    {
        center.decide();
        center.step();
    }
    return 0;
}
