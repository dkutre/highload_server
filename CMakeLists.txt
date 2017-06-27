cmake_minimum_required(VERSION 3.8)
project(highload_server)

set(CMAKE_CXX_STANDARD 14)


find_package(Boost 1.58.0 REQUIRED system thread)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    set(LIBS ${Boost_LIBRARIES})
endif()

include_directories(headers)
set(HEADER_FILES headers/Server.h headers/Connection.h headers/Reply.h headers/Header.h headers/Request.h headers/RequestHandler.h headers/RequestParser.h headers/MIME_TYPES.h)
set(SOURCE_FILES main.cpp sources/Server.cpp sources/Connection.cpp sources/Reply.cpp sources/RequestHandler.cpp sources/RequestParser.cpp sources/MIME_TYPES.cpp)
add_executable(highload_server ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(highload_server -lpthread ${LIBS})