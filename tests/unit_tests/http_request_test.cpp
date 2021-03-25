//
// Created by Chris Luttio on 3/23/21.
//

#include "gtest/gtest.h"
#include "http_request.h"

TEST(HTTP_Request_Tests, Test1) {
    HTTPRequest request;
    request.method = "Hello";
    ASSERT_EQ("Hello", request.method);
}