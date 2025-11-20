//
// Created by baizeyv on 11/20/2025.
//

#ifndef SPIDERSOLVER_MISC_MODE_H
#define SPIDERSOLVER_MISC_MODE_H
#include "../IMode.h"


class misc_mode : public IMode {
public:
    misc_mode();
    ~misc_mode() override;
    void setup() override;
    bool input() override;
private:
    bool is_input;
};


#endif //SPIDERSOLVER_MISC_MODE_H