//
// Created by Chris Luttio on 3/31/21.
//

#include "gtest/gtest.h"
#include "http_request.h"

TEST(HTTPRequestGenerationTests, Simple) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/");
    request.method = "GET";
    ASSERT_EQ("GET / HTTP/1.1\r\n", *request.generate());
}

TEST(HTTPRequestGenerationTests, Simple2) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/src/index.html");
    request.method = "POST";
    ASSERT_EQ("POST /src/index.html HTTP/1.1\r\n", *request.generate());
}

TEST(HTTPRequestGenerationTests, Headers) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/");
    request.method = "GET";
    request.headers["Content-Type"] = "text/plain";
    ASSERT_EQ("GET / HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n", *request.generate());
}


TEST(HTTPRequestGenerationTests, TwoHeaders) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/");
    request.method = "GET";
    request.headers["Content-Type"] = "text/plain";
    request.headers["Accept"] = "*";
    ASSERT_EQ("GET / HTTP/1.1\r\nAccept: *\r\nContent-Type: text/plain\r\n\r\n", *request.generate());
}

TEST(HTTPRequestGenerationTests, Body) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/");
    request.method = "GET";
    request.headers["Content-Type"] = "text/plain";
    request.body = std::make_shared<std::string>("Hello world");
    ASSERT_EQ("GET / HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world", *request.generate());
}


TEST(HTTPRequestGenerationTests, TwoHeaders_Body) {
    HTTPRequest request;
    request.version = "HTTP/1.1";
    request.uri = URL::parse("/");
    request.method = "GET";
    request.headers["Content-Type"] = "text/plain";
    request.headers["Content-Encoding"] = "hello";
    request.body = std::make_shared<std::string>("Hello world");
    ASSERT_EQ("GET / HTTP/1.1\r\nContent-Encoding: hello\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world", *request.generate());
}