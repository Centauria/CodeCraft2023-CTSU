//
// Created by Centauria V. CHEN on 2023/3/30.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include "matrix.h"
#include "system.h"
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <regex>
#include <string>

class Map : public AbstractMatrix<char>
{
public:
    Map(size_t height, size_t width);
    void append_line(const std::string &line);
    void refresh_distances();
    DMatrix get_distances();

    std::vector<System> get_systems();

    char &operator()(size_t y, size_t x) override;

private:
    std::string data;
    std::unique_ptr<DMatrix> distance_from_barriers;
    size_t width;
    size_t height;

    size_t write_pointer = 0;
    size_t rows_written = 0;

    const std::regex active_obj{R"([^.#])"};
};

#endif//CODECRAFTSDK_MAP_H
