cmake_minimum_required(VERSION 3.9)
project(P8_Web_Server)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp src/socket.cpp src/http_request_carrier.cpp src/http_request_carrier.h src/tasks/task.h src/tasks/reception_task.cpp src/tasks/reception_task.h src/state.h src/connection.cpp src/connection.h src/tasks/initialize_server_task.cpp src/tasks/initialize_server_task.h src/tasks/receive_requests_task.cpp src/tasks/receive_requests_task.h src/tasks/prune_connections_task.cpp src/tasks/prune_connections_task.h src/task_scheduler.cpp src/task_scheduler.h src/client_request.cpp src/client_request.h src/http_request_envelope.h src/tasks/process_http_requests_task.cpp src/tasks/process_http_requests_task.h src/tasks/initialize_client_requests_task.cpp src/tasks/initialize_client_requests_task.h src/tasks/load_requested_file_task.cpp src/tasks/load_requested_file_task.h src/tasks/finalize_client_requests_task.cpp src/tasks/finalize_client_requests_task.h src/tasks/send_responses_task.cpp src/tasks/send_responses_task.h src/configuration.h src/tasks/load_configuration_task.cpp src/tasks/load_configuration_task.h src/url.cpp src/url.h src/tasks/send_http_requests_task.cpp src/tasks/send_http_requests_task.h src/tasks/initialize_http_request_connections_task.cpp src/tasks/initialize_http_request_connections_task.h src/tasks/receive_http_responses_task.cpp src/tasks/receive_http_responses_task.h src/actions/action.h src/controllers/controller.h src/controllers/direct_controller.cpp src/controllers/direct_controller.h src/receivers/receiver.h src/receivers/state_action_receiver.cpp src/receivers/state_action_receiver.h src/identifiable.h src/constants.h src/objects/http_message.cpp src/objects/http_message.h src/http_message_parser.cpp src/http_message_parser.h src/applications/http_client_app.cpp src/applications/http_client_app.h src/tasks/watch_http_response_task.cpp src/tasks/watch_http_response_task.h src/applications/docker_interface_app.cpp src/applications/docker_interface_app.h src/tasks/load_routing_data_task.cpp src/tasks/load_routing_data_task.h src/objects/route.cpp src/objects/route.h src/route_resolver.cpp src/route_resolver.h src/tasks/initialize_tls_server_socket_task.cpp src/tasks/initialize_tls_server_socket_task.h src/tasks/tls_reception_task.cpp src/tasks/tls_reception_task.h src/http_request_reader.cpp src/http_request_reader.h src/objects/access_profile.cpp src/objects/access_profile.h src/objects/user_account.h src/objects/authorization.h src/tasks/authorize_user_task.cpp src/tasks/authorize_user_task.h)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})

set(SOURCE_FILES http_client.cpp src/socket.cpp src/http_request_carrier.cpp src/http_request_carrier.h src/tasks/task.h src/tasks/reception_task.cpp src/tasks/reception_task.h src/state.h src/connection.cpp src/connection.h src/tasks/initialize_server_task.cpp src/tasks/initialize_server_task.h src/tasks/receive_requests_task.cpp src/tasks/receive_requests_task.h src/tasks/prune_connections_task.cpp src/tasks/prune_connections_task.h src/task_scheduler.cpp src/task_scheduler.h src/client_request.cpp src/client_request.h src/http_request_envelope.h src/tasks/process_http_requests_task.cpp src/tasks/process_http_requests_task.h src/tasks/initialize_client_requests_task.cpp src/tasks/initialize_client_requests_task.h src/tasks/load_requested_file_task.cpp src/tasks/load_requested_file_task.h src/tasks/finalize_client_requests_task.cpp src/tasks/finalize_client_requests_task.h src/tasks/send_responses_task.cpp src/tasks/send_responses_task.h src/configuration.h src/tasks/load_configuration_task.cpp src/tasks/load_configuration_task.h src/url.cpp src/url.h src/tasks/send_http_requests_task.cpp src/tasks/send_http_requests_task.h src/tasks/initialize_http_request_connections_task.cpp src/tasks/initialize_http_request_connections_task.h src/tasks/receive_http_responses_task.cpp src/tasks/receive_http_responses_task.h src/actions/action.h src/controllers/controller.h src/controllers/direct_controller.cpp src/controllers/direct_controller.h src/receivers/receiver.h src/receivers/state_action_receiver.cpp src/receivers/state_action_receiver.h src/identifiable.h src/constants.h src/objects/http_message.cpp src/objects/http_message.h src/http_message_parser.cpp src/http_message_parser.h src/applications/http_client_app.cpp src/applications/http_client_app.h src/tasks/watch_http_response_task.cpp src/tasks/watch_http_response_task.h)
add_executable(http_client_run ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)

include_directories(lib/openssl/include/)
include_directories(lib/jwt-cpp/include/)

set(ENV{OPENSSL_ROOT_DIR} /usr/local/ssl)

find_package(OpenSSL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib OpenSSL::SSL)
target_link_libraries(http_client_run ${CMAKE_PROJECT_NAME}_lib OpenSSL::SSL)

# Doxygen Build
option(BUILD_DOC "Build Documentation" ON)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR ${CMAKE_SOURCE_DIR}/build/docs)
    if(NOT EXISTS ${BUILD_DOC_DIR})
        file(MAKE_DIRECTORY ${BUILD_DOC_DIR})
    endif()

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message("Doxygen buld started")
    add_custom_target(Doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)