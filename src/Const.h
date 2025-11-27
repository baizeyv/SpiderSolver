//
// Created by baizeyv on 2025/4/30.
//

#ifndef CONST_H
#define CONST_H
#include <string>
#include <iostream>

namespace spd {
    inline std::string VERSION = "1.0.4";

    /**
     * * 未知命令警告
     */
    static std::string UnknowCommand = "WARNING(SPD30001): [Unknow Command] -> ";

    static std::string VitaTestArgumentsException = "ERROR(SPD40000): [Vita Test Solve Arguments Error]";
    static std::string GenerationArgumentsException = "ERROR(SPD40000): [Generation Arguments Error]";
    static std::string PlayValveTestArgumentsException = "ERROR(SPD40000): [PlayValve Test Solve Arguments Error]";
    static std::string VitaTestLevelLengthException =
            "ERROR(SPD40001): [Vita Test Solve Arguments Level String Length Error]";
    static std::string PlayValveTestOptionsException = "ERROR(SPD40001): [PlayValve Test Solve Arguments String Error]";
    static std::string TaskTestArgumentsException = "ERROR(SPD40002): [Task Test Query Arguments Error]";
    static std::string StopTestArgumentsException = "ERROR(SPD40003): [Task Test Query Arguments Error]";
    static std::string TaskTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string StopTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string ViewTestArgumentsException = "ERROR(SPD40000): [View Test Solve Arguments Error]";
    static std::string ViewTestOptionsException = "ERROR(SPD40000): [View Test Solve Options Error]";
    static std::string ViewVitaLevelLengthException =
            "ERROR(SPD40001): [View Vita Arguments Level String Length Error]";
    static std::string ViewSpdSeedException = "ERROR(SPD40001): [View PlayValve Arguments Seed Error]";
    static std::string VitaTestWaitThread = "Wait Vita Test Thread End ...";
    static std::string PlayValveTestWaitThread = "Wait PlayValve Test Thread End ...";
    static std::string VitaTestThreadEnd = "Vita Test Thread End";
    static std::string PlayValveTestThreadEnd = "PlayValve Test Thread End";
    static std::string NoVitaTestTask = "No Vita Test Task";
    static std::string NoPlayValveTestTask = "No PlayValve Test Task";
    static std::string VitaTestSolved = "Vita Test Solved -> ";
    static std::string PlayValveTestSolved = "PlayValve Test Solved -> ";
    static std::string VitaTestSolving = "Vita Test Solving -> ";
    static std::string PlayValveTestSolving = "Vita Test Solving -> ";
    static std::string VitaTestStart = "Vita Test Start -> ";
    static std::string PlayValveTestStart = "PlayValve Test Start -> ";
    static std::string VitaTestRunning = "Vita Test Is Running!Please Stop It First By `stop vita`";
    static std::string StepTestRunning = "Step Test Is Running!Please Stop It First By `stop`";
    static std::string GenerationRunning = "Generation Is Running!Please Stop It First By `stop`";
    static std::string PlayValveTestRunning = "PlayValve Test Is Running!Please Stop It First By `stop vita`";

    // # part of batch mode
    static std::string BatchVitaRunning = "ERROR(SPD400001): [Batch vita mode is running! Please stop it by `stop vita` firstly.]";
    static std::string BatchVitaArgumentsException = "ERROR(SPD400000): [Batch vita solver arguments error.]";
    static std::string BatchPlayValveRunning = "ERROR(SPD500001): [Batch play-valve mode is running! Please stop it by `stop playvalve` firstly.]";
    static std::string BatchPlayValveArgumentsException = "ERROR(SPD500000): [Batch play-valve solver arguments error.]";
    static std::string BatchPGMakerRunning = "ERROR(SPD600001): [Batch pg-maker mode is running! Please stop it by `stop pgmaker` firstly.]";
    static std::string BatchPGMakerArgumentsException = "ERROR(SPD600000): [Batch pg-maker solver arguments error.]";
    static std::string BatchCustomRunning = "ERROR(SPD700001): [Batch custom mode is running! Please stop it by `stop custom` firstly.]";
    static std::string BatchCustomArgumentsException = "ERROR(SPD700000): [Batch custom solver arguments error.]";
    static std::string BatchStopArgumentsException = "ERROR(SPD800000): [Batch stop command arguments error.]";
    static std::string BatchStopOptionsException = "ERROR(SPD800001): [Batch stop command options error.]";
    static std::string BatchVitaSolved = "Vita solved -> ";
    static std::string BatchVitaSolving = "Vita solving -> ";
    static std::string BatchPlayValveSolved = "PlayValve solved -> ";
    static std::string BatchPlayValveSolving = "PlayValve solving -> ";
    static std::string BatchPGMakerSolved = "PGMaker solved -> ";
    static std::string BatchPGMakerSolving = "PGMaker solving -> ";
    static std::string BatchCustomSolved = "Custom solved -> ";
    static std::string BatchCustomSolving = "Custom solving -> ";
    static std::string BatchVitaWaitThread = "Wait Batch Vita Thread End ...";
    static std::string BatchVitaThreadEnd = "Batch Vita Thread End";
    static std::string BatchPlayValveWaitThread = "Wait Batch PlayValve Thread End ...";
    static std::string BatchPlayValveThreadEnd = "Batch PlayValve Thread End";
    static std::string BatchPGMakerWaitThread = "Wait Batch PGMaker Thread End ...";
    static std::string BatchPGMakerThreadEnd = "Batch PGMaker Thread End";
    static std::string BatchCustomWaitThread = "Wait Batch Custom Thread End ...";
    static std::string BatchCustomThreadEnd = "Batch Custom Thread End";
    static std::string Calc = " ->CALC:";

    inline std::string EmptyCard = ":::: ";

    inline bool DebugOutput = false;
    inline std::string OutputPath;

    static void output_icon() {
        std::cout <<
                "   _____ ____  ________  __________ " << std::endl <<
                R"(  / ___// __ \/  _/ __ \/ ____/ __ \)" << std::endl <<
                "  \\__ \\/ /_/ // // / / / __/ / /_/ /" << std::endl <<
                " ___/ / ____// // /_/ / /___/ _, _/ " << std::endl <<
                "/____/_/   /___/_____/_____/_/ |_|  " << std::endl <<
                "\033[34m     Welcome to Spider Solver!\033[0m" << std::endl;
    }
}

#endif //CONST_H
