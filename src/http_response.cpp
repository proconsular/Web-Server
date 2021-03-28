//
// Created by Chris Luttio on 3/25/21.
//

#include <vector>
#include "utils.h"
#include "http_response.h"

std::string* HTTPResponse::generate() {
    std::vector<std::string> request_line = {version, std::to_string(code), status};

    std::vector<std::string> headers_output;
    for (std::map<std::string, std::string>::const_iterator i = headers.begin(); i != headers.end(); i++) {
        headers_output.push_back(join(": ", {i->first, i->second}));
    }

    int body_size = 0;
    if (body != NULL) {
        body_size = body->size();
    }
    headers_output.push_back(join(": ", {"Content-Length", std::to_string(body_size)}));

    return new std::string(join("\r\n", {join(" ", request_line), join("\r\n", headers_output), "", body != NULL ? *body : ""}));
}