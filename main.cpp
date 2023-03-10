#include "src/center.h"
#include "src/io.h"
#include <iostream>
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