//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_DIRECT_CONTROLLER_H
#define P8_WEB_SERVER_DIRECT_CONTROLLER_H

#include "controller.h"

class DirectController: public Controller {
public:

    void apply(Action *) override;
};


#endif //P8_WEB_SERVER_DIRECT_CONTROLLER_H
