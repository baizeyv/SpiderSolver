//
// Created by baizeyv on 2025/4/30.
//

#ifndef TESTMODE_H
#define TESTMODE_H
#include <thread>

#include "../IMode.h"
#include "../Solver.h"


class TestMode final : public IMode
{
public:
    TestMode();
    ~TestMode() override;
    void setup() override;
    bool input() override;
    /**
     * 
     * @param type 0->vita and playvalve 1->playvalve 2->vita
     */
    void join(int type) ;
private:
    bool is_input;

    // #region vita test
    
    Solver* vita_test_solver;
    std::unique_ptr<std::thread> vita_test_thread;
    std::atomic<bool> vita_test_thread_done = false;

    // #region playvalve test
    Solver* playvalve_test_solver;
    std::unique_ptr<std::thread> playvalve_test_thread;
    std::atomic<bool> playvalve_test_thread_done = false;
    
};

#endif //TESTMODE_H
