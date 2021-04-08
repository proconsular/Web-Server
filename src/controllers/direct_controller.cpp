//
// Created by Chris Luttio on 4/4/21.
//

#include "direct_controller.h"

#include <utility>
#include "receivers/state_action_receiver.h"
#include "receivers/log_action_receiver.h"

DirectController::DirectController(std::shared_ptr<State> state): Controller(state) {
    _receivers.push_back(new StateActionReceiver(state));
    _receivers.push_back(new LogActionReceiver("log.txt"));
}

void DirectController::apply(const Action& action) {
    for (auto receiver: _receivers) {
        receiver->receive(action);
    }
}