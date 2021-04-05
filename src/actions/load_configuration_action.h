//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_LOAD_CONFIGURATION_ACTION_H
#define P8_WEB_SERVER_LOAD_CONFIGURATION_ACTION_H

#include "action.h"

#include <utility>
#include "configuration.h"

class LoadConfigurationAction: public Action {
public:
    explicit LoadConfigurationAction(Configuration config): Action(SetConfiguration), config(std::move(config)) {}

    Configuration config;
};

#endif //P8_WEB_SERVER_LOAD_CONFIGURATION_ACTION_H
