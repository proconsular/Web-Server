//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_PROCESS_HTTP_REQUESTS_TASK_H
#define P8_WEB_SERVER_PROCESS_HTTP_REQUESTS_TASK_H

#include "task.h"
#include "state.h"

class ProcessHTTPRequestsTask: public Task {
public:
    explicit ProcessHTTPRequestsTask(State* state): state(state), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }

private:
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_PROCESS_HTTP_REQUESTS_TASK_H
