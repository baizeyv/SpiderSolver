cmake_minimum_required(VERSION 3.31)
project(SpiderSolver)

set(CMAKE_CXX_STANDARD 20)

add_executable(spider main.cpp
        Card.cpp
        Card.h
        Poker.cpp
        Poker.h
        State.cpp
        State.h
        Solver.cpp
        Solver.h
        Helper.cpp
        Helper.h
        cxxopts.h
)
