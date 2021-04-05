//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_FINALIZE_CLIENT_REQUESTS_TASK_H
#define P8_WEB_SERVER_FINALIZE_CLIENT_REQUESTS_TASK_H

#include "task.h"
#include "state.h"

class FinalizeClientRequestsTask: public Task {
public:
    FinalizeClientRequestsTask(State* state): state(state), _alive(true) {}

    void perform() override;
    bool alive() override {
        return _alive;
    }

private:
    std::string getContentType(const std::string&);

    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_FINALIZE_CLIENT_REQUESTS_TASK_H
