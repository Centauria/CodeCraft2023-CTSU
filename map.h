//
// Created by Centauria V. CHEN on 2023/3/30.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

class Map
{
public:
    Map(size_t height, size_t width);
    void append_line(const std::string &line);

private:
    std::string data;
    size_t write_pointer = 0;
    size_t width = 0;
    size_t height = 0;
};

#endif//CODECRAFTSDK_MAP_H
