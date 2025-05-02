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
        src/mode/TestMode.cpp
        src/mode/TestMode.h
        src/Const.h
        src/exporter/Exporter.cpp
        src/exporter/Exporter.h
        src/data/LevelData.cpp
        src/data/LevelData.h
        src/mode/DebugMode.cpp
        src/mode/DebugMode.h
        src/mode/BatchMode.cpp
        src/mode/BatchMode.h
        src/json.hpp
        src/data/VitaBean.cpp
        src/data/VitaBean.h
        src/data/LevelBean.cpp
        src/data/LevelBean.h
        src/resource.h
        res/SpiderSolver.rc
)
