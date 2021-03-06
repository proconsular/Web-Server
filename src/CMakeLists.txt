project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        socket.h
        utils.h
        )

set(SOURCE_FILES
        socket.cpp
        json.hpp
        utils.cpp tasks/task.h tasks/reception_task.cpp tasks/reception_task.h state.h connection.cpp connection.h tasks/initialize_server_task.cpp tasks/initialize_server_task.h tasks/receive_requests_task.cpp tasks/receive_requests_task.h tasks/prune_connections_task.cpp tasks/prune_connections_task.h task_scheduler.cpp task_scheduler.h client_request.cpp client_request.h http_request_envelope.h tasks/process_http_requests_task.cpp tasks/process_http_requests_task.h tasks/initialize_client_requests_task.cpp tasks/initialize_client_requests_task.h tasks/load_requested_file_task.cpp tasks/load_requested_file_task.h tasks/finalize_client_requests_task.cpp tasks/finalize_client_requests_task.h http_response_envelope.h tasks/send_responses_task.cpp tasks/send_responses_task.h configuration.h tasks/load_configuration_task.cpp tasks/load_configuration_task.h url.cpp url.h tasks/send_http_requests_task.cpp tasks/send_http_requests_task.h http_request_carrier.cpp http_request_carrier.h tasks/initialize_http_request_connections_task.cpp tasks/initialize_http_request_connections_task.h tasks/receive_http_responses_task.cpp tasks/receive_http_responses_task.h actions/action.h controllers/controller.h controllers/direct_controller.cpp controllers/direct_controller.h receivers/receiver.h receivers/state_action_receiver.cpp receivers/state_action_receiver.h receivers/log_action_receiver.cpp receivers/log_action_receiver.h identifiable.h constants.h objects/http_message.cpp objects/http_message.h http_message_parser.cpp http_message_parser.h applications/http_client_app.cpp applications/http_client_app.h tasks/watch_http_response_task.cpp tasks/watch_http_response_task.h applications/docker_interface_app.cpp applications/docker_interface_app.h tasks/load_routing_data_task.cpp tasks/load_routing_data_task.h objects/route.cpp objects/route.h route_resolver.cpp route_resolver.h tasks/initialize_tls_server_socket_task.cpp tasks/initialize_tls_server_socket_task.h tasks/tls_reception_task.cpp tasks/tls_reception_task.h http_request_reader.cpp http_request_reader.h objects/access_profile.cpp objects/access_profile.h objects/user_account.h objects/authorization.h tasks/authorize_user_task.cpp tasks/authorize_user_task.h objects/web_socket_message.h tasks/send_web_socket_messages_task.cpp tasks/send_web_socket_messages_task.h web_socket_message_generator.cpp web_socket_message_generator.h tasks/recieve_web_socket_messages_task.cpp tasks/recieve_web_socket_messages_task.h web_socket_message_parser.cpp web_socket_message_parser.h objects/session.h objects/session_record.h objects/web_socket_message_frame.h objects/web_socket_message_header.h)

include_directories(../lib/openssl/include/)
include_directories(../lib/jwt-cpp/include/)

#if (EXISTS ${SOURCE_FILES})
#    # The library contains header and source files.
#
#else()
#    # The library only contains header files.
#    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})