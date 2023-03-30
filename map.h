//
// Created by Centauria V. CHEN on 2023/3/30.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include "matrix.h"
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <regex>
#include <string>

class Map
{
public:
    Map(size_t height, size_t width);
    void append_line(const std::string &line);
    void refresh_distances();

private:
    std::string data;
    std::unique_ptr<DMatrix> distance_from_barriers;
    size_t width = 0;
    size_t height = 0;

    size_t write_pointer = 0;
    size_t rows_written = 0;

    const std::regex active_obj{R"([^.#])"};
};

#endif//CODECRAFTSDK_MAP_H
