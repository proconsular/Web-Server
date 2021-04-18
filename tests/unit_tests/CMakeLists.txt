include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Unit_Tests_run
        http_request_test.cpp http_request_parser_test.cpp http_response_test.cpp url_tests.cpp http_request_tests.cpp http_response_parser_tests.cpp http_response_progressive_parser_tests.cpp copy_tests.cpp http_server_tests.cpp)

target_link_libraries(Unit_Tests_run gtest gtest_main)
target_link_libraries(Unit_Tests_run ${CMAKE_PROJECT_NAME}_lib)