//
// Created by Centauria V. CHEN on 2023/4/7.
//

#ifndef CODECRAFTSDK_IO_H
#define CODECRAFTSDK_IO_H

#include "concept.h"
#include "matrix.h"

std::vector<std::string> input_map();
bool input_frame(std::vector<WorkBench> &, std::vector<Robot> &, Timer &timer);
CMatrix read_map(const std::vector<std::string> &map);
std::vector<WorkBench> read_workbenches(const std::vector<std::string> &map);
std::vector<Robot> read_robots(const std::vector<std::string> &map);

#endif//CODECRAFTSDK_IO_H
