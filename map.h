//
// Created by Centauria V. CHEN on 2023/3/30.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include <cstdint>
#include <cstdlib>
#include <memory>

class Map
{
public:
    Map(size_t width, size_t height);
    void append_line();

private:
    std::unique_ptr<char *> data;
    size_t width = 0;
    size_t height = 0;
};

#endif//CODECRAFTSDK_MAP_H
