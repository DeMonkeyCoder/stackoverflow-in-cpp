cmake_minimum_required(VERSION 3.12)
project(stackoverflow_in_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(stackoverflow_in_cpp
    AbstractUser.h
    Exceptions.h
    main.cpp
    User.cpp
    User.h)
