//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H
#define P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H

#include "task.h"
#include "controllers/controller.h"

class InitializeServerTask: public Task {
public:
    explicit InitializeServerTask(State* state, Controller* controller): _state(state), _controller(controller), _alive(true) {};

    void perform() override;

    bool alive() override {
        return _alive;
    }
private:
    const State* _state;
    Controller* _controller;
    bool _alive;
};


#endif //P8_WEB_SERVER_INITIALIZE_SERVER_TASK_H
