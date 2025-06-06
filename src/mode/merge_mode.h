//
// Created by baizeyv on 6/6/2025.
//

#ifndef MERGE_MODE_H
#define MERGE_MODE_H
#include "../IMode.h"


class merge_mode : public IMode {
public:
    merge_mode();
    ~merge_mode() override;
    void setup() override;
    bool input() override;
private:
    bool is_input;
};



#endif //MERGE_MODE_H
