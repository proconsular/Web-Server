//
// Created by Chris Luttio on 3/23/21.
//

#include "http_request.h"
#include "utils.h"

std::shared_ptr<std::string> HTTPRequest::generate() const {
    auto output = std::make_shared<std::string>();

    std::vector<std::string> request_line = {method, uri.to_string(), version};

    output->append(join(" ", request_line));
    output->append("\r\n");

    if (!headers.empty()) {
        std::vector<std::string> _headers;
        for (const auto& pair: headers) {
            _headers.push_back(join(": ", {pair.first, pair.second}));
        }
        if (body != nullptr && !body->empty()) {
            _headers.push_back(join(": ", {"Content-Length", std::to_string(body->size())}));
        }
        output->append(join("\r\n", _headers));
        output->append("\r\n\r\n");
    }

    if (body != nullptr && !body->empty()) {
        output->append(*body);
    }

    return output;
}