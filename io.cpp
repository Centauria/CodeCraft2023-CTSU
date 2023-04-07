//
// Created by Centauria V. CHEN on 2023/4/7.
//

#include "io.h"
#include "function.h"

#include <iostream>

std::vector<std::string> input_map()
{
    std::vector<std::string> result;
    std::string line;
    while (true)
    {
        std::cin >> std::ws >> line;
        if (std::cin.eof() || line == "OK") break;
        result.emplace_back(line);
    }
    return result;
}

CMatrix read_map(const std::vector<std::string> &map)
{
    CMatrix result{100, 100, true};
    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (map[j][i] == '#')
            {
                result(j, i) = 0;
            } else
            {
                result(j, i) = 3;
            }
        }
    }
    return result;
}
std::vector<std::string> input_frame()
{
    std::vector<std::string> result;
    std::string line;

    return result;
}
std::vector<WorkBench> read_workbenches(const std::vector<std::string> &map)
{
    std::vector<WorkBench> result;
    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            auto c = map[j][i];
            if ('1' <= c && c <= '9')
            {
                WorkBench r;
                r.coordinate = {j, i};
                r.type = int(c - '0');
                result.emplace_back(r);
            }
        }
    }
    return result;
}
std::vector<Robot> read_robots(const std::vector<std::string> &map)
{
    std::vector<Robot> result;
    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            auto c = map[j][i];
            if (c == 'A')
            {
                Robot r;
                r.position = get_point(Index{j, i});
                r.id = int(result.size());
                result.emplace_back(r);
            }
        }
    }
    return result;
}