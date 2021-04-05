//
// Created by Chris Luttio on 3/31/21.
//

#include "gtest/gtest.h"
#include "http_response.h"

TEST(HTTPResponseParsingTests, RequestLine) {
    auto data = new std::string("HTTP/1.1 200 OK");
    auto* response = HTTPResponse::parse(data);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
}

TEST(HTTPResponseParsingTests, Header) {
    auto data = new std::string("HTTP/1.1 200 OK\r\nContent-Type: text/plain");
    auto* response = HTTPResponse::parse(data);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("text/plain", response->headers["Content-Type"]);
}


TEST(HTTPResponseParsingTests, TwoHeader) {
    auto data = new std::string("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nHello: World");
    auto* response = HTTPResponse::parse(data);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("text/plain", response->headers["Content-Type"]);
    ASSERT_EQ("World", response->headers["Hello"]);
}

TEST(HTTPResponseParsingTests, Body) {
    auto data = new std::string("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world");
    auto* response = HTTPResponse::parse(data);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("text/plain", response->headers["Content-Type"]);
    ASSERT_EQ("11", response->headers["Content-Length"]);
    ASSERT_EQ("Hello world", *response->body);
}

