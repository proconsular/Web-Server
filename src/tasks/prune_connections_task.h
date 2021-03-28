//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H
#define P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H

#include "task.h"
#include "state.h"

class PruneConnectionsTask: public Task {
public:
    PruneConnectionsTask(State* state): state(state), _alive(true) {};

    void perform() override;

    bool alive() override {
        return _alive;
    }
private:
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_PRUNE_CONNECTIONS_TASK_H
