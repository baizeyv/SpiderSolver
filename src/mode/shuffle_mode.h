//
// Created by baizeyv on 5/23/2025.
//

#ifndef SHUFFLE_MODE_H
#define SHUFFLE_MODE_H
#include "../IMode.h"


class shuffle_mode : public IMode {
public:
    shuffle_mode();
    ~shuffle_mode() override;
    void setup() override;
    bool input() override;
private:
    bool is_input;
};



#endif //SHUFFLE_MODE_H
