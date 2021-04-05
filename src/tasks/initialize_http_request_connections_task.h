//
// Created by Chris Luttio on 4/1/21.
//

#ifndef P8_WEB_SERVER_INITIALIZE_HTTP_REQUEST_CONNECTIONS_TASK_H
#define P8_WEB_SERVER_INITIALIZE_HTTP_REQUEST_CONNECTIONS_TASK_H

#include "task.h"
#include "state.h"

class InitializeHTTPRequestConnectionsTask: public Task {
public:
    explicit InitializeHTTPRequestConnectionsTask(State* state): _state(state), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    State* _state;
    bool _alive;
};


#endif //P8_WEB_SERVER_INITIALIZE_HTTP_REQUEST_CONNECTIONS_TASK_H
