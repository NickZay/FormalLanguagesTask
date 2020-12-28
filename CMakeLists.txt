cmake_minimum_required(VERSION 3.17)
project(Formals)

set(CMAKE_CXX_STANDARD 20)

add_executable(main solution.cpp solution.h main.cpp)
#add_executable(Formals solution.cpp solution.h tests.cpp)
