#include <csignal>
#include <iostream>

#include "Const.h"
#include "cxxopts.h"
#include "json.hpp"
#include "Poker.h"
#include "data/VitaBean.h"
#include "mode/BatchMode.h"
#include "mode/DebugMode.h"
#include "mode/TestMode.h"

/**
 * * Main Function
 * @param argc 
 * @param argv 
 * @return 
 */
int main(const int argc, char* argv[])
{
    system("cls");
    spd::output_icon();

    cxxopts::Options options("spider", "Test Desc");
    options.add_options()
        ("h,help", spd::HelpOptionsDescription)
        ("t,test", "test mode")
        ("d,debug", "debug mode")
        ("b,batch", "batch mode")
        ("o,output", "debug output path", cxxopts::value<std::string>()->default_value(""))
        ("v,version", "print version");
    try
    {
        if (const auto result = options.parse(argc, argv); result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }
        else if (result.count("test"))
        {
            // # test mode
            const auto mode = new TestMode();
            mode->setup();
            mode->enter();
            delete mode;
        }
        else if (result.count("debug"))
        {
            // # debug mode
            if (result.count("output"))
            {
                spd::OutputPath = result["output"].as<std::string>();
            }
            spd::DebugOutput = true;
            const auto mode = new DebugMode();
            mode->setup();
            mode->enter();
            delete mode;
        }
        else if (result.count("batch"))
        {
            // # batch mode
            const auto mode = new BatchMode();
            mode->setup();
            mode->enter();
            delete mode;
        } else if (result.count("version"))
        {
            std::cout << "       spider version:" << spd::VERSION << std::endl;
        }
    }
    catch (const cxxopts::exceptions::exception e)
    {
        std::cerr << "Error " << e.what() << std::endl;
    }

    return 0;
}
