project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        socket.h
        http_request.h
        )

set(SOURCE_FILES
        socket.cpp
        http_request.cpp
        http_request_parser.cpp http_request_parser.h utils.h)

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