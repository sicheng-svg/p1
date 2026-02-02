cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MYSQL REQUIRED mysqlclient)

add_executable(minesweeper src/main.cpp src/database.cpp)
target_include_directories(minesweeper PRIVATE include ${MYSQL_INCLUDE_DIRS})
target_link_libraries(minesweeper ${MYSQL_LIBRARIES} pthread)
target_link_directories(minesweeper PRIVATE ${MYSQL_LIBRARY_DIRS})
