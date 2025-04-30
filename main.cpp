#include <csignal>
#include <iostream>

#include "cxxopts.h"
#include "Poker.h"
#include "Solver.h"
#include "State.h"

void handle_signal(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n 程序已被中断 (CTRL+C). Exiting..." << std::endl;
        exit(0);
    }
}

int main(int argc, char* argv[]) {

    // # 处理 CTRL+C 退出
    std::signal(SIGINT, handle_signal);

    // auto poker = new Poker(36655, 3);
    // const auto solver = new Solver("JCALQZ,1;MZWQYI,1;VTGKLP,1;LOISTE,1;LORVD,1;WNAXU,1;FFISM,1;GUBBI,1;TAXPR,1;YMEYS,1;TJPGCAOFWRKVXCDQHHNGOJZXPUHBVSKNYZKBEWEQUCFJRHDNMD,0");
    // solver->test_dfs();
    // delete solver;

    // system("cls");
    cxxopts::Options options("spider", "Test Desc");
    options.add_options()("h,help", "print help");
    try {
        auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }
    } catch (const cxxopts::exceptions::exception e) {
        std::cerr << "Error " << e.what() << std::endl;
    }

    while (true) {
        
    }

    // std::cout << "Count:" << argc << std::endl;
    // for (size_t i = 0; i < argc; i ++) {
    //     std::cout << argv[i] << std::endl;
    // }


    return 0;
}