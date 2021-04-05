//
// Created by Chris Luttio on 3/31/21.
//

#ifndef P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H
#define P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H

#include "task.h"
#include "state.h"

class SendHTTPRequestsTask: public Task {
public:
    explicit SendHTTPRequestsTask(State* state): _state(state), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    State* _state;
    bool _alive;
};


#endif //P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H
