cmake_minimum_required(VERSION 3.16)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(src)
file(GLOB SOURCES "src/*.cpp")

add_executable(minesweeper ${SOURCES})
target_link_libraries(minesweeper sfml-graphics sfml-window sfml-system)
