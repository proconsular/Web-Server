//
// Created by Chris Luttio on 3/25/21.
//

#include "gtest/gtest.h"
#include "http_response.h"
#include "utils.h"

TEST(UtilsTest, Join1) {
    ASSERT_EQ("hello", join(", ", {"hello"}));
}

TEST(HTTPResponseGenerationTests, Output1) {
    HTTPResponse ok = HTTPResponse::OK();
    ASSERT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n", ok.generate());
}

TEST(HTTPResponseGenerationTests, Output2) {
    HTTPResponse ok = HTTPResponse::OK();
    ok.body = "Hello world";
    ASSERT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello world", ok.generate());
}

TEST(HTTPResponseGenerationTests, Output_Headers) {
    HTTPResponse ok = HTTPResponse::OK();
    ok.body = "Hello world";
    ok.headers["Content-Type"] = "text/plain";
    ok.headers["Cache-Control"] = "no-cache";
    ok.headers["Host"] = "localhost:8080";
    ASSERT_EQ("HTTP/1.1 200 OK\r\nCache-Control: no-cache\r\nContent-Type: text/plain\r\nHost: localhost:8080\r\nContent-Length: 11\r\n\r\nHello world", ok.generate());
}