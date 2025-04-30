//
// Created by baizeyv on 2025/4/30.
//

#ifndef CONST_H
#define CONST_H
#include <string>

namespace spd
{
    static std::string HelpOptionsDescription = "help desc"; // TODO:
    static std::string UnknowCommand = "WARNING(SPD30001): [Unknow Command] -> ";
    static std::string VitaTestArgumentsException = "ERROR(SPD40000): [Vita Test Solve Arguments Error]";
    static std::string VitaTestLevelLengthException = "ERROR(SPD40001): [Vita Test Solve Arguments Level String Length Error]";
    static std::string TaskTestArgumentsException = "ERROR(SPD40002): [Task Test Query Arguments Error]";
    static std::string StopTestArgumentsException = "ERROR(SPD40003): [Task Test Query Arguments Error]";
    static std::string TaskTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string StopTestOptionsException = "ERROR(SPD40004): [Task Test Query Options Error]";
    static std::string VitaTestWaitThread = "Wait Vita Test Thread End ...";
    static std::string VitaTestThreadEnd = "Vita Test Thread End";
    static std::string NoVitaTestTask = "No Vita Test Task";
    static std::string VitaTestSolved = "Vita Test Solved -> ";
    static std::string VitaTestSolving = "Vita Test Solving -> ";
    static std::string VitaTestStart = "Vita Test Start -> ";
    static std::string VitaTestRunning = "Vita Test Is Running!Please Stop It First By `stop vita`";
    static void output_icon()
    {
        std::cout <<
            "   _____ ____  ________  __________ " << std::endl <<
            R"(  / ___// __ \/  _/ __ \/ ____/ __ \)" << std::endl <<
            "  \\__ \\/ /_/ // // / / / __/ / /_/ /" << std::endl <<
            " ___/ / ____// // /_/ / /___/ _, _/ " << std::endl <<
            "/____/_/   /___/_____/_____/_/ |_|  " << std::endl <<
            "Welcome to Spider Solver!" << std::endl;
    }
}

#endif //CONST_H
