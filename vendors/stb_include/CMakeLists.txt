cmake_minimum_required(VERSION 3.25)
project(stb_include)

add_library(stb_include STATIC)

set(SOURCES
        stb_include/stb_include.cpp
        stb_include/stb_include.h
        )

target_sources(stb_include PRIVATE ${SOURCES})
target_include_directories(stb_include PUBLIC .)

# C++17
set_property(TARGET stb_include PROPERTY CXX_STANDARD 17)
