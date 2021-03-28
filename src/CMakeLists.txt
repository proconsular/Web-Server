project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        socket.h
        http_request.h
        http_request_parser.h
        utils.h
        http_response.h
        )

set(SOURCE_FILES
        socket.cpp
        http_request.cpp
        http_request_parser.cpp
        http_response.cpp
        utils.cpp tasks/task.h tasks/reception_task.cpp tasks/reception_task.h state.h connection.cpp connection.h tasks/initialize_server_task.cpp tasks/initialize_server_task.h tasks/receive_requests_task.cpp tasks/receive_requests_task.h tasks/prune_connections_task.cpp tasks/prune_connections_task.h task_scheduler.cpp task_scheduler.h client_request.cpp client_request.h http_request_envelope.h tasks/process_http_requests_task.cpp tasks/process_http_requests_task.h tasks/initialize_client_requests_task.cpp tasks/initialize_client_requests_task.h tasks/load_requested_file_task.cpp tasks/load_requested_file_task.h tasks/finalize_client_requests_task.cpp tasks/finalize_client_requests_task.h http_response_envelope.h tasks/send_responses_task.cpp tasks/send_responses_task.h configuration.h)

#if (EXISTS ${SOURCE_FILES})
#    # The library contains header and source files.
#
#else()
#    # The library only contains header files.
#    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()

add_library(${CMAKE_PROJECT_NAME}_lib STATIC
        ${SOURCE_FILES}
        ${HEADER_FILES}
        )