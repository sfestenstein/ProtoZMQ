cmake_minimum_required(VERSION 3.5)

project(ProtoMsgs LANGUAGES CXX)

find_package(Protobuf REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CPP_GEN_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

INCLUDE(FindProtobuf)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/inc)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/generated)
FILE (GLOB ALL_PROTO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/proto/*.proto)
FILE (GLOB ALL_SOURCE_HEADERS "inc/*.h")
FILE (GLOB ALL_SOURCE_SOURCES "src/*.cpp")
PROTOBUF_GENERATE_CPP(PROTOSOURCES PROTOHEADERS ${ALL_PROTO_SOURCES})

foreach (FILE ${PROTOSOURCES} ${PROTOHEADERS})
    get_filename_component(BASENAME ${FILE} NAME)
    set(OUT_FILE ${CPP_GEN_DIR}/${BASENAME})
    add_custom_command(
        OUTPUT ${OUT_FILE}
        DEPENDS ${FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CPP_GEN_DIR}
        COMMAND cp -p ${FILE} ${OUT_FILE})
    list(APPEND PROTO_FILES ${OUT_FILE})
endforeach()

add_library(ProtoMsgs SHARED
    ${ALL_MSG_SOURCES}
    ${ALL_MSG_HEADERS}
    ${PROTO_FILES} ${ALL_PROTO_SOURCES}
    ${ALL_SOURCE_HEADERS}
    ${ALL_SOURCE_SOURCES}
)

add_executable(TestPublisher TestPublisher.cpp)
target_link_libraries(TestPublisher ProtoMsgs zmq protobuf)

add_executable(TestSubscriber TestSubscriber.cpp)
target_link_libraries(TestSubscriber ProtoMsgs zmq protobuf)

add_executable(ZmqRouter ZmqRouter.cpp)
target_link_libraries(ZmqRouter zmq)

install(TARGETS ProtoMsgs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
