cmake_minimum_required(VERSION 3.31)
project(SpiderSolver)

set(CMAKE_CXX_STANDARD 20)

add_executable(spider src/main.cpp
        src/Card.cpp
        src/Card.h
        src/Poker.cpp
        src/Poker.h
        src/State.cpp
        src/State.h
        src/Solver.cpp
        src/Solver.h
        src/Helper.cpp
        src/Helper.h
        src/cxxopts.h
        src/IMode.h
        src/TestMode.cpp
        src/TestMode.h
        src/Const.h
)
