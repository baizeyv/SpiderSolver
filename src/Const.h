//
// Created by baizeyv on 2025/4/30.
//

#ifndef CONST_H
#define CONST_H
#include <string>
#include <iostream>

namespace spd
{
    inline std::string VERSION = "1.0.3";
    
    static std::string UnknowCommand = "WARNING(SPD30001): [Unknow Command] -> ";
    static std::string VitaTestArgumentsException = "ERROR(SPD40000): [Vita Test Solve Arguments Error]";
    static std::string GenerationArgumentsException = "ERROR(SPD40000): [Generation Arguments Error]";
    static std::string PlayValveTestArgumentsException = "ERROR(SPD40000): [PlayValve Test Solve Arguments Error]";
    static std::string VitaTestLevelLengthException = "ERROR(SPD40001): [Vita Test Solve Arguments Level String Length Error]";
    static std::string PlayValveTestOptionsException = "ERROR(SPD40001): [PlayValve Test Solve Arguments String Error]";
    static std::string TaskTestArgumentsException = "ERROR(SPD40002): [Task Test Query Arguments Error]";
    static std::string StopTestArgumentsException = "ERROR(SPD40003): [Task Test Query Arguments Error]";
    static std::string TaskTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string StopTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string ViewTestArgumentsException = "ERROR(SPD40000): [View Test Solve Arguments Error]";
    static std::string ViewTestOptionsException = "ERROR(SPD40000): [View Test Solve Options Error]";
    static std::string ViewVitaLevelLengthException = "ERROR(SPD40001): [View Vita Arguments Level String Length Error]";
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
    
    inline std::string EmptyCard = ":::: ";

    inline bool DebugOutput = false;
    inline std::string OutputPath;
    
    static void output_icon()
    {
        std::cout <<
            "   _____ ____  ________  __________ " << std::endl <<
            R"(  / ___// __ \/  _/ __ \/ ____/ __ \)" << std::endl <<
            "  \\__ \\/ /_/ // // / / / __/ / /_/ /" << std::endl <<
            " ___/ / ____// // /_/ / /___/ _, _/ " << std::endl <<
            "/____/_/   /___/_____/_____/_/ |_|  " << std::endl <<
            "     Welcome to Spider Solver!" << std::endl;
    }
}

#endif //CONST_H
