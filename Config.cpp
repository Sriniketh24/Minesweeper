#include "Config.h"
#include <fstream>
#include <stdexcept>

Config::Config(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Cannot open config file: " + filename);
    in >> cols >> rows >> mines;
}
