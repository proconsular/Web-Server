//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H
#define P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H

#include "task.h"

#include <utility>
#include "client_request.h"
#include "configuration.h"
#include "controllers/controller.h"

class LoadRequestedFileTask: public Task {
public:
    explicit LoadRequestedFileTask(Controller* controller, ClientRequest* request, Configuration config): _controller(controller), _request(request), config(std::move(config)), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }

    Configuration config;
private:
    Controller* _controller;
    ClientRequest* _request;
    bool _alive;
};


#endif //P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H
