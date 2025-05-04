//
// Created by baizeyv on 2025/5/4.
//

#ifndef GENMODE_H
#define GENMODE_H
#include <thread>
#include "../IMode.h"
#include "../Solver.h"


class GenMode : public IMode
{
public:
    GenMode();
    ~GenMode() override;
    void setup() override;
    bool input() override;
    void join() ;
private:
    bool is_input;

    Solver* gen_solver;
    std::unique_ptr<std::thread> gen_thread;
    std::atomic<bool> gen_thread_done = false;
    bool gen_stop_flag = false;
};


#endif //GENMODE_H
