cmake_minimum_required(VERSION 3.17)
project(sockets_with_C C)

set(CMAKE_C_STANDARD 99)

add_executable(sockets_with_C client.c server.c)