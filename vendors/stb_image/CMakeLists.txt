cmake_minimum_required(VERSION 3.25)
project(stb_image)

add_library(stb_image STATIC)

set(SOURCES
        stb_image/stb_image.cpp
        stb_image/stb_image.h
        )

target_sources(stb_image PRIVATE ${SOURCES})
target_include_directories(stb_image PUBLIC .)

# C++17
set_property(TARGET stb_image PROPERTY CXX_STANDARD 17)