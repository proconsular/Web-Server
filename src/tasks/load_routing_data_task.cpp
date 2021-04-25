//
// Created by Chris Luttio on 4/24/21.
//

#include "load_routing_data_task.h"
#include "objects/route.h"
#include "json.hpp"

#include <dirent.h>
#include <cstdio>
#include <fstream>
#include <algorithm>

void LoadRoutingDataTask::perform() {
    DIR *d;
    struct dirent *dir;
    const char* dir_name = _state->config->routing_directory.c_str();
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != nullptr) {
            size_t e = strlen(dir->d_name);
            size_t n = e;
            while (dir->d_name[n] != '.') n--;
            if (strcmp("json", dir->d_name + n + 1) != 0)
                continue;

            auto path = std::string(dir_name) + dir->d_name;
            std::fstream file(path);

            nlohmann::json data;
            file >> data;

            file.close();

            Route route;

            route.url = data["Route"];
            route.default_file = data["Default"];
            route.cache_control = data["Cache-Control"];

            auto content_type = data["Content-Type"];
            if (content_type.is_string()) {
                route.content_type[content_type] = "";
            } else if (content_type.is_object()) {
                for (auto iter = content_type.begin(); iter != content_type.end(); iter++) {
                    route.content_type[iter.key()] = iter.value();
                }
            }

            auto language = data["Content-Language"];
            if (language.is_string()) {
                route.language[language] = "";
            } else if (language.is_object()) {
                for (auto iter = language.begin(); iter != language.end(); iter++) {
                    route.language[iter.key()] = iter.value();
                }
            }

            _state->routes[route.url] = route;
        }
        closedir(d);
    }
    _alive = false;
}