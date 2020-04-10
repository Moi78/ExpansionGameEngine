cmake_minimum_required(VERSION 3.0)

project(ExpansionMath)

file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(ExpansionMath SHARED ${SRCS} ${HEADERS})

target_include_directories(ExpansionMath PUBLIC)
