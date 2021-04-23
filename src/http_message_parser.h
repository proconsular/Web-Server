//
// Created by Chris Luttio on 4/14/21.
//

#ifndef P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H
#define P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H

#include <iostream>
#include <memory>

#include "objects/http_message.h"

enum ReadMode {
    STATUS,
    HEADERS,
    BODY,
    CHUNKED,
};

class HttpMessageParser {
public:
    explicit HttpMessageParser(HttpMessageType type): type(type), cursor(0), body_length(0), mode(STATUS), done(false) {
        message = std::make_shared<HttpMessage>(type);
        built_up_message = std::make_shared<std::string>();
        built_up_body = std::make_shared<std::string>();
    }

    bool partial_parse(const char* buffer, size_t amount);

    [[nodiscard]] std::shared_ptr<HttpMessage> get_message() const {
        return message;
    }

    void finalize() {
        if (mode == CHUNKED) {
            message->body = built_up_body;
        }
    }

private:
    HttpMessageType type;

    std::shared_ptr<HttpMessage> message;

    std::shared_ptr<std::string> built_up_message;
    std::shared_ptr<std::string> built_up_body;

    int cursor;
    int body_length;

    bool done;

    ReadMode mode;

    bool parse_headers();
    bool parse_chunked_body();

    template<typename T>
    static int parse_request_status_line(T start, T end, const std::shared_ptr<HttpMessage>& request) {
        std::vector<std::string> words;
        std::string word;

        auto i = start;
        for (; i != end; i++) {
            if (*i == '\r')
                break;
            if (!isspace(*i)) {
                word += *i;
            } else {
                words.push_back(word);
                word = "";
            }
        }

        if (words.size() == 2 && !word.empty()) {
            words.push_back(word);
        }

        request->method = words[0];
        request->url = URL::parse(words[1]);
        request->version = words[2];

        if (i != end && *i == '\r') {
            i += 2;
        }

        return i - start;
    }

    template <typename T>
    static int parse_response_status_line(T start, T end, const std::shared_ptr<HttpMessage>& response) {
        auto cursor = start;
        auto mark = cursor;
        std::vector<std::string> words;
        int c = 0;
        for (; cursor != end; cursor++) {
            if (*cursor == '\r' || *cursor == '\n')
                break;
            if (c < 2 && isspace(*cursor)) {
                words.emplace_back(mark, cursor);
                mark = cursor + 1;
                c++;
            }
        }
        response->version = words[0];
        response->code = atoi(words[1].c_str());
        response->status = std::string(mark, cursor);
        while (cursor != end && !isalnum(*cursor)) cursor++;
        return cursor - start;
    }

    template<typename T>
    static int parse_header(T start, T end, const std::shared_ptr<HttpMessage>& response) {
        auto cursor = start;
        auto beginning = cursor;
        while (cursor != end && *cursor != ':') cursor++;
        if (cursor == end) {
            return -1;
        }
        auto key = std::string(beginning, cursor);
        beginning = cursor + 1;
        while (isspace(*beginning)) beginning++;
        while (cursor != end && *cursor != '\0' && *cursor != '\r' && *cursor != '\n') cursor++;
        if (cursor == end || *cursor == '\0') {
            return -1;
        }
        response->headers[key] = std::make_shared<std::string>(beginning, cursor);
        if ((*cursor == '\n' && cursor + 1 < end) || (*cursor == '\r' && cursor + 2 < end)) {
            cursor += *cursor == '\n' ? 1 : 2;
        } else {
            return -1;
        }
        return cursor - start;
    }

    static int parse_chunk_header(const std::shared_ptr<std::string>&, int, int&);
    static int parse_chunk(const std::shared_ptr<std::string>&, int, int, const std::shared_ptr<HttpMessage>&);
};


#endif //P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H
