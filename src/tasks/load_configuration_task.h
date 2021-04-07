//
// Created by Chris Luttio on 3/28/21.
//

#ifndef P8_WEB_SERVER_LOAD_CONFIGURATION_TASK_H
#define P8_WEB_SERVER_LOAD_CONFIGURATION_TASK_H

#include "task.h"
#include "state.h"
#include "controllers/controller.h"

class LoadConfigurationTask: public Task {
public:
    explicit LoadConfigurationTask(Controller* controller): _controller(controller), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    Controller* _controller;
    bool _alive;
};


#endif //P8_WEB_SERVER_LOAD_CONFIGURATION_TASK_H
