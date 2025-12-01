//
// Created by baizeyv on 2025/4/30.
//

#ifndef CONST_H
#define CONST_H
#include <iostream>
#include <string>

namespace spd {
    inline std::string VERSION = "1.0.4";

    /**
     * * 未知命令警告
     */
    static std::string UnknowCommand = "WARNING(SPD30001): [Unknow Command] -> ";

    static std::string VitaTestArgumentsException = "ERROR(SPD40000): [Vita Test Solve Arguments Error]";
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
    static std::string PlayValveTestRunning = "PlayValve Test Is Running!Please Stop It First By `stop vita`";

    static std::string CompleteDesc = "[Completed !!!]";

    // # part of batch mode
    static std::string BatchVitaRunning =
            "ERROR(SPD400001): [Batch vita mode is running! Please stop it by `stop vita` firstly.]";
    static std::string BatchVitaArgumentsException = "ERROR(SPD400000): [Batch vita solver arguments error.]";
    static std::string BatchPlayValveRunning =
            "ERROR(SPD500001): [Batch play-valve mode is running! Please stop it by `stop playvalve` firstly.]";
    static std::string BatchPlayValveArgumentsException =
            "ERROR(SPD500000): [Batch play-valve solver arguments error.]";
    static std::string BatchPGMakerRunning =
            "ERROR(SPD600001): [Batch pg-maker mode is running! Please stop it by `stop pgmaker` firstly.]";
    static std::string BatchPGMakerArgumentsException = "ERROR(SPD600000): [Batch pg-maker solver arguments error.]";
    static std::string BatchDoodleRunning =
            "ERROR(SPD700001): [Batch doodle mode is running! Please stop it by `stop doodle` firstly.]";
    static std::string BatchDoodleArgumentsException = "ERROR(SPD700000): [Batch doodle solver arguments error.]";
    static std::string BatchStopArgumentsException = "ERROR(SPD800000): [Batch stop command arguments error.]";
    static std::string BatchStopOptionsException = "ERROR(SPD800001): [Batch stop command options error.]";
    static std::string BatchVitaSolved = "Vita solved -> ";
    static std::string BatchVitaSolving = "Vita solving -> ";
    static std::string BatchPlayValveSolved = "PlayValve solved -> ";
    static std::string BatchPlayValveSolving = "PlayValve solving -> ";
    static std::string BatchPGMakerSolved = "PGMaker solved -> ";
    static std::string BatchPGMakerSolving = "PGMaker solving -> ";
    static std::string BatchDoodleSolved = "Doodle solved -> ";
    static std::string BatchDoodleSolving = "Doodle solving -> ";
    static std::string BatchVitaWaitThread = "Wait Batch Vita Thread End ...";
    static std::string BatchVitaThreadEnd = "Batch Vita Thread End";
    static std::string BatchPlayValveWaitThread = "Wait Batch PlayValve Thread End ...";
    static std::string BatchPlayValveThreadEnd = "Batch PlayValve Thread End";
    static std::string BatchPGMakerWaitThread = "Wait Batch PGMaker Thread End ...";
    static std::string BatchPGMakerThreadEnd = "Batch PGMaker Thread End";
    static std::string BatchDoodleWaitThread = "Wait Batch Doodle Thread End ...";
    static std::string BatchDoodleThreadEnd = "Batch Doodle Thread End";
    static std::string Calc = " ->CALC:";

    // # part of debug mode
    static std::string DebugVitaRunning =
            "ERROR(SPD400001): [Debug vita mode is running! Please stop it by `stop vita` firstly.]";
    static std::string DebugVitaArgumentsException = "ERROR(SPD400000): [Debug vita solver arguments error.]";
    static std::string DebugVitaLevelLengthException = "ERROR(SPD400002): [Debug vita mode level string length error.]";
    static std::string DebugVitaStart = "Vita Debug Start -> ";
    static std::string DebugPlayValveRunning =
            "ERROR(SPD400001): [Debug play-valve mode is running! Please stop it by `stop playvalve` firstly.]";
    static std::string DebugPlayValveArgumentsException =
            "ERROR(SPD400000): [Debug play-valve solver arguments error.]";
    static std::string DebugPlayValveStart = "PlayValve Debug Start -> ";
    static std::string DebugPGMakerRunning =
            "ERROR(SPD400001): [Debug pg-maker mode is running! Please stop it by `stop pgmaker` firstly.]";
    static std::string DebugPGMakerArgumentsException = "ERROR(SPD400000): [Debug pg-maker solver arguments error.]";
    static std::string DebugPGMakerStart = "PGMaker Debug Start -> ";
    static std::string DebugStopArgumentsException = "ERROR(SPD800000): [Debug stop command arguments error.]";
    static std::string DebugStopOptionsException = "ERROR(SPD800001): [Debug stop command options error.]";
    static std::string DebugDoodleRunning =
            "ERROR(SPD400001): [Debug doodle mode is running! Please stop it by `stop doodle` firstly.]";
    static std::string DebugDoodleArgumentsException = "ERROR(SPD400000): [Debug doodle solver arguments error.]";
    static std::string DebugDoodleLevelLengthException =
            "ERROR(SPD400002): [Debug doodle mode level string length error.]";
    static std::string DebugDoodleStart = "Doodle Debug Start -> ";
    static std::string DebugVitaWaitThread = "Wait Debug Vita Thread End ...";
    static std::string DebugVitaThreadEnd = "Debug Vita Thread End";
    static std::string DebugPlayValveWaitThread = "Wait Debug PlayValve Thread End ...";
    static std::string DebugPlayValveThreadEnd = "Debug PlayValve Thread End";
    static std::string DebugPGMakerWaitThread = "Wait Debug PGMaker Thread End ...";
    static std::string DebugPGMakerThreadEnd = "Debug PGMaker Thread End";
    static std::string DebugDoodleWaitThread = "Wait Debug Doodle Thread End ...";
    static std::string DebugDoodleThreadEnd = "Debug Doodle Thread End";

    // # part of test mode
    static std::string TestVitaRunning =
            "ERROR(SPD400001): [Test vita mode is running! Please stop it by `stop vita` firstly.]";
    static std::string TestViewArgumentsException = "ERROR(SPD40000): [View Test Solve Arguments Error]";
    static std::string TestViewVitaLevelLengthException =
            "ERROR(SPD40001): [View Vita Arguments Level String Length Error]";
    static std::string TestVitaArgumentsException = "ERROR(SPD400000): [Test vita solver arguments error.]";
    static std::string TestVitaLevelLengthException = "ERROR(SPD400002): [Test vita mode level string length error.]";
    static std::string TestVitaStart = "Vita Test Start -> ";
    static std::string TestPlayValveRunning =
            "ERROR(SPD400001): [Test play-valve mode is running! Please stop it by `stop playvalve` firstly.]";
    static std::string TestPlayValveArgumentsException = "ERROR(SPD400000): [Test play-valve solver arguments error.]";
    static std::string TestPlayValveStart = "PlayValve Test Start -> ";
    static std::string TestPGMakerRunning =
            "ERROR(SPD400001): [Test pg-maker mode is running! Please stop it by `stop pgmaker` firstly.]";
    static std::string TestPGMakerArgumentsException = "ERROR(SPD400000): [Test pg-maker solver arguments error.]";
    static std::string TestPGMakerStart = "PGMaker Test Start -> ";
    static std::string TestDoodleRunning =
            "ERROR(SPD400001): [Test doodle mode is running! Please stop it by `stop doodle` firstly.]";
    static std::string TestDoodleArgumentsException = "ERROR(SPD400000): [Test doodle solver arguments error.]";
    static std::string TestDoodleLevelLengthException =
            "ERROR(SPD400002): [Test doodle mode level string length error.]";
    static std::string TestDoodleStart = "Doodle Test Start -> ";
    static std::string TestStopArgumentsException = "ERROR(SPD800000): [Test stop command arguments error.]";
    static std::string TestStopOptionsException = "ERROR(SPD800001): [Test stop command options error.]";
    static std::string TestQueryArgumentsException = "ERROR(SPD800000): [Test query command arguments error.]";
    static std::string TestQueryOptionsException = "ERROR(SPD800001): [Test query command options error.]";
    static std::string TestVitaSolved = "Vita solved -> ";
    static std::string TestVitaSolving = "Vita solving -> ";
    static std::string TestVitaNull = "No Vita Test Task.";
    static std::string TestPlayValveSolved = "PlayValve solved -> ";
    static std::string TestPlayValveSolving = "PlayValve solving -> ";
    static std::string TestPlayValveNull = "No PlayValve Test Task.";
    static std::string TestPGMakerSolved = "PGMaker solved -> ";
    static std::string TestPGMakerSolving = "PGMaker solving -> ";
    static std::string TestPGMakerNull = "No PGMaker Test Task.";
    static std::string TestDoodleSolved = "Doodle solved -> ";
    static std::string TestDoodleSolving = "Doodle solving -> ";
    static std::string TestDoodleNull = "No Doodle Test Task.";

    // # part of generation mode
    static std::string GenRunning = "Generation is running! Please stop it by `stop` firstly";
    static std::string GenArgumentsException = "ERROR(SPD40000): [Generation Arguments Error]";
    static std::string GenWaitThread = "Wait Thread End ...";
    static std::string GenThreadEnd = "Generation Thread End";

    inline std::string EmptyCard = ":::: ";

    inline bool DebugOutput = false;
    inline std::string OutputPath;

    static void output_icon() {
        std::cout << "   _____ ____  ________  __________ " << std::endl
                  << R"(  / ___// __ \/  _/ __ \/ ____/ __ \)" << std::endl
                  << "  \\__ \\/ /_/ // // / / / __/ / /_/ /" << std::endl
                  << " ___/ / ____// // /_/ / /___/ _, _/ " << std::endl
                  << "/____/_/   /___/_____/_____/_/ |_|  " << std::endl
                  << "\033[34m     Welcome to Spider Solver!\033[0m" << std::endl;
    }
} // namespace spd

#endif // CONST_H
