cmake_minimum_required(VERSION 3.22)

add_library(BulldozerFileManager SHARED
        BD_Reader.cpp BD_Reader.h
        BD_Writer.cpp BD_Writer.h)

target_include_directories(BulldozerFileManager PUBLIC "../ExpansionMath")
target_link_libraries(BulldozerFileManager ExpansionMath)