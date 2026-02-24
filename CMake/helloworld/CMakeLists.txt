# 1.指定该CMake项目的最低要求的CMake版本，避免旧版本直接运行项目导致部分内容出错
cmake_minimum_required(VERSION 3.18)

# 2.指定项目名称
project(helloWorld)

# 3.添加可执行目标
add_executable(main main.cpp)