//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_INITIALIZE_CLIENT_REQUESTS_TASK_H
#define P8_WEB_SERVER_INITIALIZE_CLIENT_REQUESTS_TASK_H

#include "task.h"
#include "state.h"
#include "controllers/controller.h"

class InitializeClientRequestsTask: public Task {
public:
    explicit InitializeClientRequestsTask(State* state, Controller* controller): state(state), _controller(controller), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    Controller* _controller;
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_INITIALIZE_CLIENT_REQUESTS_TASK_H
