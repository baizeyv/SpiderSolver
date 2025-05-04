//
// Created by baizeyv on 2025/5/4.
//

#ifndef STEPMODE_H
#define STEPMODE_H
#include <thread>

#include "../IMode.h"
#include "../Solver.h"


class StepMode final : public IMode {
public:
    StepMode();
    ~StepMode() override;
    void setup() override;
    bool input() override;
    void join() ;
private:
    bool is_input;

    Solver* step_solver;
    std::unique_ptr<std::thread> step_thread;
    std::atomic<bool> step_thread_done = false;
};



#endif //STEPMODE_H
