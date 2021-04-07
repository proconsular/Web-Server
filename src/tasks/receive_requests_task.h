//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_RECEIVE_REQUESTS_TASK_H
#define P8_WEB_SERVER_RECEIVE_REQUESTS_TASK_H

#include "task.h"
#include "state.h"
#include "controllers/controller.h"

class ReceiveRequestsTask: public Task {
public:
    explicit ReceiveRequestsTask(State* state, Controller* controller): state(state), _controller(controller), _alive(true) {};

    void perform() override;

    bool alive() override {
        return _alive;
    }
private:
    State* state;
    Controller* _controller;
    bool _alive;
};


#endif //P8_WEB_SERVER_RECEIVE_REQUESTS_TASK_H
