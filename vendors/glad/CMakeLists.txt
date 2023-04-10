cmake_minimum_required(VERSION 3.25)
project(glad)

add_library(glad STATIC)

set(SOURCES
        glad/glad.cpp
        glad/glad.h
        glad/khrplatform.h
        )

target_sources(glad PRIVATE ${SOURCES})
target_include_directories(glad PUBLIC .)

# C++17
set_property(TARGET glad PROPERTY CXX_STANDARD 17)
