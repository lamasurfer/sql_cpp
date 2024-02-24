cmake_minimum_required(VERSION 3.27)
project(task_5)

set(CMAKE_CXX_STANDARD 20)

add_executable(task_5 main.cpp
        ClientRepo.cpp
        ClientRepo.h
        Phone.h
        Client.h
        Client.cpp
        Phone.cpp
)

add_subdirectory(lib/libpqxx build-pqxx)
target_link_libraries(task_5 PRIVATE pqxx)