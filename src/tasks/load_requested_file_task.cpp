//
// Created by Chris Luttio on 3/27/21.
//

#include "load_requested_file_task.h"

#include <iostream>
#include <cstdio>
#include <unistd.h>

void LoadRequestedFileTask::perform() {
    auto url = config.base_url + _request->uri.substr(1);
    if (_request->uri == "/") {
        url = config.base_url + "index.html";
    }
    auto name = url.c_str();
    if (access(name, F_OK) == 0) {
        FILE* file = fopen(name, "rb");
        auto* output = new std::string;
        const int BUFFER_SIZE = 1024 * 4;
        size_t amount_read;
        do {
            char buffer[BUFFER_SIZE + 1] = {0};
            amount_read = fread(buffer, sizeof(char), BUFFER_SIZE, file);
            output->append(buffer);
        } while (amount_read == BUFFER_SIZE);
        fclose(file);
        _request->data = output;
        _request->status = RequestStatus::Complete;
    } else {
        _request->status = RequestStatus::Failed;
    }
    _alive = false;
}