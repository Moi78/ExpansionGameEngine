cmake_minimum_required(VERSION 3.22)

add_library(ExpansionMath SHARED
        vec.cpp vec.h mat4.cpp mat4.h quat.h SIMD_Utils.h)