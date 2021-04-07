//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_RECEPTION_TASK_H
#define P8_WEB_SERVER_RECEPTION_TASK_H

#include "task.h"
#include "state.h"
#include "controllers/controller.h"

class ReceptionTask: public Task {
public:
    explicit ReceptionTask(State* state, Controller* controller): state(state), _controller(controller), _alive(true) {}

    void perform();

    bool alive() {
        return _alive;
    }
private:
    Controller* _controller;
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_RECEPTION_TASK_H
