//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H
#define P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H

#include "task.h"
#include "state.h"
#include "controllers/controller.h"

class PruneConnectionsTask: public Task {
public:
    PruneConnectionsTask(State* state, Controller* controller): state(state), _controller(controller), _alive(true) {};

    void perform() override;

    bool alive() override {
        return _alive;
    }
private:
    Controller* _controller;
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H
