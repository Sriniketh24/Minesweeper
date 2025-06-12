#pragma once
#include <string>

struct Config {
    int cols;
    int rows;
    int mines;
    Config(const std::string& filename);
};
