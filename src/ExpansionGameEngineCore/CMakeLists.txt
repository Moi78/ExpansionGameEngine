cmake_minimum_required(VERSION 3.22)

add_library(ExpansionGameEngineCore SHARED
        EXP_Game.cpp EXP_Game.h)

target_link_libraries(ExpansionGameEngineCore RaindropRenderer)
target_link_libraries(ExpansionGameEngineCore ExpansionMath)
target_link_libraries(ExpansionGameEngineCore BulldozerFileManager)