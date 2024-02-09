cmake_minimum_required(VERSION 3.27)
project(ProgChallenge)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32 wsock32)

add_executable(ProgChallenge main.cpp
        sender.c
        middle.c
        receiver.c
        winHead.h
        middleman.c
)
