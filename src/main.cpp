#include <csignal>
#include <iostream>

#include "Const.h"
#include "cxxopts.h"
#include "Poker.h"
#include "TestMode.h"

int main(int argc, char* argv[])
{
    system("cls");
    spd::output_icon();

    cxxopts::Options options("spider", "Test Desc");
    options.add_options()("h,help", spd::HelpOptionsDescription)("t,test", "test mode");
    try
    {
        auto result = options.parse(argc, argv);
        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }
        else if (result.count("test"))
        {
            const auto mode = new TestMode();
            mode->setup();
            mode->enter();
            delete mode;
        }
    }
    catch (const cxxopts::exceptions::exception e)
    {
        std::cerr << "Error " << e.what() << std::endl;
    }


    return 0;
}
