cmake_minimum_required(VERSION 3.16)
project(ColorSpaceConversion)

set(CMAKE_CXX_STANDARD 20)

add_executable(ColorSpaceConversion main.cpp CImage.cpp CImage.h CException.cpp CException.h)