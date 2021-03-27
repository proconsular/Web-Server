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
        utils.cpp tasks/task.h tasks/reception_task.cpp tasks/reception_task.h state.cpp state.h connection.cpp connection.h tasks/initialize_server_task.cpp tasks/initialize_server_task.h tasks/receive_requests_task.cpp tasks/receive_requests_task.h tasks/prune_connections_task.cpp tasks/prune_connections_task.h task_scheduler.cpp task_scheduler.h client_request.cpp client_request.h)

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