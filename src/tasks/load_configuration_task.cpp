//
// Created by Chris Luttio on 3/28/21.
//

#include <iostream>
#include <istream>
#include <fstream>

#include "load_configuration_task.h"
#include "json.hpp"
#include "configuration.h"

using json = nlohmann::json;

void LoadConfigurationTask::perform() {
    auto filename = "config.json";
    std::ifstream file(filename);
    if (!file.fail()) {
        json config;
        file >> config;
        file.close();
        auto new_config = std::make_shared<Configuration>();
        if (config.find("base_url") != config.end()) {
            new_config->base_url = URL::parse(config["base_url"]);
        }
        if (config.find("port") != config.end()) {
            new_config->port = config["port"];
        }
        if (config.find("keep-alive") != config.end()) {
            new_config->keep_alive = config["keep-alive"];
        }
        if (config.find("port_fixed") != config.end()) {
            new_config->port_fixed = config["port_fixed"];
        }
        if (config.find("routes") != config.end()) {
            new_config->routing_directory = config["routes"];
        }
        new_config->from_file = filename;
        _controller->apply(Action(SetConfiguration, new_config));
    }
    _alive = false;
}