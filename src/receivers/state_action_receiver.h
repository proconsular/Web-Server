//
// Created by Chris Luttio on 4/5/21.
//

#ifndef P8_WEB_SERVER_STATE_ACTION_RECEIVER_H
#define P8_WEB_SERVER_STATE_ACTION_RECEIVER_H

#include "receiver.h"
#include "state.h"

class StateActionReceiver: public Receiver {
public:
    explicit StateActionReceiver(State* state): _state(state) {}

    void receive(const Action &) override;

private:
    State* _state;
};


#endif //P8_WEB_SERVER_STATE_ACTION_RECEIVER_H
