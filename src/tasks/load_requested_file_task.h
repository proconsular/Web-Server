//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H
#define P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H

#include "task.h"
#include "client_request.h"
#include "configuration.h"

class LoadRequestedFileTask: public Task {
public:
    LoadRequestedFileTask(ClientRequest* request, const Configuration& config): _request(request), config(config), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }

    Configuration config;
private:
    ClientRequest* _request;
    bool _alive;
};


#endif //P8_WEB_SERVER_LOAD_REQUESTED_FILE_TASK_H
