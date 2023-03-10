#include "src/center.h"
using namespace std;

int main()
{
    Center center;
    center.initialize();
    while (center.refresh())
    {
        center.step();
    }
    return 0;
}
