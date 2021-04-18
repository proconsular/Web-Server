//
// Created by Chris Luttio on 4/14/21.
//

#ifndef P8_WEB_SERVER_HTTP_RESPONSE_PARSER_H
#define P8_WEB_SERVER_HTTP_RESPONSE_PARSER_H

#include "http_response.h"
#include <memory>

enum ReadMode {
    STATUS,
    HEADERS,
    BODY,
    CHUNKED,
};

class HttpResponseParser {
public:
    HttpResponseParser(): cursor(0), body_length(0), mode(STATUS) {
        response = std::make_shared<HTTPResponse>();
        built_up_message = std::make_shared<std::string>();
        built_up_body = std::make_shared<std::string>();
    }

    bool partial_parse(const char* buffer, ssize_t amount);
    [[nodiscard]] std::shared_ptr<HTTPResponse> get_response() const {
        return response;
    }

    void finalize() {
        if (mode == CHUNKED) {
            response->body = built_up_body;
        }
    }

private:
    std::shared_ptr<HTTPResponse> response;

    std::shared_ptr<std::string> built_up_message;
    std::shared_ptr<std::string> built_up_body;

    int cursor;
    int body_length;

    ReadMode mode;

    bool parse_headers();
    bool parse_chunked_body();
};


#endif //P8_WEB_SERVER_HTTP_RESPONSE_PARSER_H
