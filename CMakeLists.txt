# cmake_minimum_required(VERSION <specify CMake version here>)
project(stackoverflow_in_cpp1)

set(CMAKE_CXX_STANDARD 14)

add_executable(stackoverflow_in_cpp1 main.cpp AbstractUser.h Exceptions.h User.cpp User.h)