//
// Created by Chris Luttio on 4/14/21.
//

#include "http_response_parser.h"

bool HttpResponseParser::partial_parse(const char *buffer, ssize_t amount) {
    built_up_message->append(std::string(buffer, buffer + amount));

    while (cursor < built_up_message->size() && mode != BODY) {
        switch (mode) {
            case STATUS: {
                cursor += HTTPResponse::parse_status_line(built_up_message->begin(), built_up_message->end(), response);
                mode = HEADERS;
                break;
            }
            case HEADERS: {
                bool ok = parse_headers();
                if (!ok)
                    return true;
                break;
            }
            case CHUNKED: {
                return parse_chunked_body();
            }
            default:
                return true;
        }
    }

    if (mode == BODY && cursor + body_length == built_up_message->size()) {
        response->body = std::__1::make_shared<std::string>(built_up_message->begin() + cursor, built_up_message->begin() + cursor + body_length);
        return false;
    }

    return true;
}

bool HttpResponseParser::parse_headers() {
    int length = HTTPResponse::parse_header(built_up_message->begin() + cursor, built_up_message->end(), response);
    if (length == -1) {
        return false;
    }
    cursor += length;
    if (cursor < built_up_message->size()) {
        if (built_up_message->at(cursor) == '\r' || built_up_message->at(cursor) == '\n') {
            if (response->headers.find("Content-Length") != response->headers.end()) {
                body_length = atoi(response->headers["Content-Length"]->c_str());
                mode = BODY;
            } else if (response->headers.find("Transfer-Encoding") != response->headers.end()) {
                mode = CHUNKED;
            }
            if (built_up_message->at(cursor) == '\r') {
                cursor += 2;
            } else {
                cursor += 1;
            }
        }
    }
    return true;
}

bool HttpResponseParser::parse_chunked_body() {
    int size;
    int length = HTTPResponse::parse_chunk_header(built_up_message, cursor, size);
    if (length == -1) {
        return true;
    }
    if (size == 0) {
        response->body = built_up_body;
        return false;
    }
    if (cursor + length + size >= built_up_message->size()) {
        return true;
    }
    cursor += length;
    built_up_body->append(std::string(built_up_message->begin() + cursor, built_up_message->begin() + cursor + size));
    cursor += size + 2;
    return true;
}
