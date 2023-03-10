//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include "io.h"
#include <iostream>


bool readUntilOK()
{
    char line[1024];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
        //do something
    }
    return false;
}