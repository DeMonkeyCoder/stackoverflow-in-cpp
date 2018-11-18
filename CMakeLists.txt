cmake_minimum_required(VERSION 3.12)
project(stackoverflow_in_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(stackoverflow_in_cpp
    AbstractUser.h
    CMakeLists.txt
    Design.png
    Exceptions.h
    LICENSE
    main.cpp
    README.md
    User.cpp
    User.h)
