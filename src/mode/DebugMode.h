//
// Created by baizeyv on 2025/5/1.
//

#ifndef DEBUGMODE_H
#define DEBUGMODE_H
#include "../IMode.h"
#include <thread>
#include "../Solver.h"


class DebugMode : public IMode {
public:
    DebugMode();

    ~DebugMode() override;

    void setup() override;

    bool input() override;

    /**
     * 
     * @param type 0->vita and playvalve and pgmaker 1->playvalve 2->vita 3->pgmaker 4->custom
     */
    void join(int type);

private:
    bool is_input;

    // #region vita test

    Solver *vita_debug_solver;
    std::unique_ptr<std::thread> vita_debug_thread;
    std::atomic<bool> vita_debug_thread_done = false;

    // #region playvalve test
    Solver *playvalve_debug_solver;
    std::unique_ptr<std::thread> playvalve_debug_thread;
    std::atomic<bool> playvalve_debug_thread_done = false;

    // #region pgmaker test
    Solver *pgmaker_debug_solver;
    std::unique_ptr<std::thread> pgmaker_debug_thread;
    std::atomic<bool> pgmaker_debug_thread_done = false;

    // #region custom test
    Solver *doodle_debug_solver;
    std::unique_ptr<std::thread> doodle_debug_thread;
    std::atomic<bool> doodle_debug_thread_done = false;
};


#endif //DEBUGMODE_H
