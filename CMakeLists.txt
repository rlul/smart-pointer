cmake_minimum_required(VERSION 3.10)

project(smart-pointer)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD 20)

add_executable(smart-pointer main.cpp  "testing.h" "smartptr/weakptr.h" "smartptr/sharedptr.h" "smartptr/controlblock.h")
