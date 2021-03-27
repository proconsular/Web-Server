//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_RECEPTION_TASK_H
#define P8_WEB_SERVER_RECEPTION_TASK_H

#include "task.h"
#include "state.h"

class ReceptionTask: public Task {
public:
    ReceptionTask(State* state) {
        this->state = state;
        _alive = true;
    }

    void perform();

    bool alive() {
        return _alive;
    }
private:
    State* state;
    bool _alive;
};


#endif //P8_WEB_SERVER_RECEPTION_TASK_H
