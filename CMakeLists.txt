cmake_minimum_required(VERSION 3.31)
project(SpiderSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")

add_executable(spider src/main.cpp
        src/Card.cpp
        src/Card.h
        src/Poker.cpp
        src/Poker.h
        src/State.cpp
        src/State.h
        src/Solver.cpp
        src/Solver.h
        src/Joker.cpp
        src/Joker.h
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
        src/HistoryItem.h
        src/HistoryItem.cpp
        src/IMemUsage.h
        src/mode/GenMode.cpp
        src/mode/GenMode.h
        src/data/LevelBean2.cpp
        src/data/LevelBean2.h
        src/data/VitaBean2.cpp
        src/data/VitaBean2.h
        src/data/level_data.cpp
        src/data/level_data.h
        src/mode/StepMode.cpp
        src/mode/StepMode.h
        src/mode/level_mode.cpp
        src/mode/level_mode.h
        src/parser/csv_parser.cpp
        src/parser/csv_parser.h
        src/parser/csv_data.cpp
        src/parser/csv_data.h
        src/exporter/shuffle_exporter.cpp
        src/exporter/shuffle_exporter.h
        src/exporter/level_exporter.cpp
        src/exporter/level_exporter.h
        src/data/shuffle_data.cpp
        src/data/shuffle_data.h
        src/mode/shuffle_mode.cpp
        src/mode/shuffle_mode.h
        src/mode/misc_mode.cpp
        src/mode/misc_mode.h
)

#set(UPX_EXE "E:\\env\\upx-5.0.2-win64\\upx.exe")
#add_custom_command(TARGET spider POST_BUILD
#        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:spider>
#        COMMAND ${UPX_EXE} --best --lzma --force $<TARGET_FILE:spider>
#        COMMENT "Stripping and UPX compressing spider"
#)
