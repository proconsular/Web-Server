//
// Created by Chris Luttio on 3/28/21.
//

#include <iostream>
#include <istream>
#include <fstream>

#include "load_configuration_task.h"
#include "json.hpp"

using json = nlohmann::json;

void LoadConfigurationTask::perform() {
    std::ifstream file("config.json");
    if (!file.fail()) {
        json config;
        file >> config;
        file.close();
        if (config.find("base_url") != config.end()) {
            _state->config.base_url = URL::parse(config["base_url"]);
        }
        if (config.find("port") != config.end()) {
            _state->config.port = config["port"];
        }
        if (config.find("keep-alive") != config.end()) {
            _state->config.keep_alive = config["keep-alive"];
        }
    }
    _alive = false;
}