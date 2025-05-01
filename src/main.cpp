#include <csignal>
#include <iostream>

#include "Const.h"
#include "cxxopts.h"
#include "Poker.h"
#include "TestMode.h"

int main(int argc, char* argv[])
{
    // std::signal(SIGINT, handle_signal);

    // auto poker = new Poker(36655, 3);
    // const auto solver = new Solver("JCALQZ,1;MZWQYI,1;VTGKLP,1;LOISTE,1;LORVD,1;WNAXU,1;FFISM,1;GUBBI,1;TAXPR,1;YMEYS,1;TJPGCAOFWRKVXCDQHHNGOJZXPUHBVSKNYZKBEWEQUCFJRHDNMD,0");
    // solver->test_dfs();
    // delete solver;

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
