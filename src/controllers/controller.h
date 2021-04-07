//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_CONTROLLER_H
#define P8_WEB_SERVER_CONTROLLER_H

#include "state.h"
#include "actions/action.h"

class Controller {
public:
    explicit Controller(State* state): _state(state) {}

    virtual void apply(const Action&) = 0;
protected:
    State* _state;
};

#endif //P8_WEB_SERVER_CONTROLLER_H
