include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Unit_Tests_run
        url_tests.cpp copy_tests.cpp http_server_tests.cpp http_message_parser_tests.cpp http_message_generation_tests.cpp load_routing_data_task_tests.cpp parse_accept_header_tests.cpp)

target_link_libraries(Unit_Tests_run gtest gtest_main)
target_link_libraries(Unit_Tests_run ${CMAKE_PROJECT_NAME}_lib)