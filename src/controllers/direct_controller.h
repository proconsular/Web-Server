//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_DIRECT_CONTROLLER_H
#define P8_WEB_SERVER_DIRECT_CONTROLLER_H

#include "controller.h"
#include "receivers/receiver.h"

class DirectController: public Controller {
public:
    explicit DirectController(State* state);

    void apply(const Action&) override;
private:
    std::vector<Receiver*> _receivers;
};


#endif //P8_WEB_SERVER_DIRECT_CONTROLLER_H
