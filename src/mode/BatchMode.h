//
// Created by baizeyv on 2025/5/2.
//

#ifndef BATCHMODE_H
#define BATCHMODE_H
#include <thread>
#include "../IMode.h"
#include "../Solver.h"


class BatchMode : public IMode {
public:
    BatchMode();
    ~BatchMode() override;
    void setup() override;
    bool input() override;
    /**
     * 
     * @param type 0->vita and playvalve and pgmaker 1->playvalve 2->vita 3->pgmaker
     */
    void join(int type) ;
private:
    bool is_input;
    
    // #region vita test
    
    Solver* vita_batch_solver;
    std::unique_ptr<std::thread> vita_batch_thread;
    std::atomic<bool> vita_batch_thread_done = false;
    bool vita_batch_stop_flag = false;

    // #region playvalve test
    Solver* playvalve_batch_solver;
    std::unique_ptr<std::thread> playvalve_batch_thread;
    std::atomic<bool> playvalve_batch_thread_done = false;
    bool playvalve_batch_stop_flag = false;

    // #region pgmaker test
    Solver* pgmaker_batch_solver;
    std::unique_ptr<std::thread> pgmaker_batch_thread;
    std::atomic<bool> pgmaker_batch_thread_done = false;
    bool pgmaker_batch_stop_flag = false;
};



#endif //BATCHMODE_H
