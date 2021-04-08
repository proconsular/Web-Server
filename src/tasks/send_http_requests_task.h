//
// Created by Chris Luttio on 3/31/21.
//

#ifndef P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H
#define P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H

#include "task.h"

#include <utility>
#include "state.h"

class SendHTTPRequestsTask: public Task {
public:
    explicit SendHTTPRequestsTask(std::shared_ptr<State> state): _state(std::move(state)), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    std::shared_ptr<State> _state;
    bool _alive;
};


#endif //P8_WEB_SERVER_SEND_HTTP_REQUESTS_TASK_H
