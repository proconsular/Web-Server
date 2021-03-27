//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H
#define P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H

#include "task.h"
#include "state.h"

class InitializeServerTask: public Task {
public:
    InitializeServerTask(State* state): state(state) {};

    void perform() override;

    bool alive() override {
        return false;
    }
private:
    State* state;
};


#endif //P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H
