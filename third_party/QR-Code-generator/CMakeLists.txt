cmake_minimum_required(VERSION 3.5)
project(QR-Code-generator LANGUAGES CXX)

add_library(QR-Code-generator
    include/QrCode.hpp
    src/QrCode.cpp)
target_include_directories(QR-Code-generator PUBLIC include)
