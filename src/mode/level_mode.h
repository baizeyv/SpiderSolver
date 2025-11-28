//
// Created by baizeyv on 11/20/2025.
//

#ifndef SPIDERSOLVER_LEVEL_MODE_H
#define SPIDERSOLVER_LEVEL_MODE_H
#include "../IMode.h"


class level_mode final : public IMode {
public:
    level_mode();
    ~level_mode() override;
    void setup() override;
    bool input() override;
private:
    bool is_input;
};


#endif //SPIDERSOLVER_LEVEL_MODE_H