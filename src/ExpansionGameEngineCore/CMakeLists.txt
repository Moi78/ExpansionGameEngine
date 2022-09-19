cmake_minimum_required(VERSION 3.22)

add_library(ExpansionGameEngineCore SHARED
        EXP_Game.cpp EXP_Game.h EXP_Conf.h EXP_HotLoad.cpp EXP_HotLoad.h EXP_MapLoader.cpp EXP_MapLoader.h)

target_link_libraries(ExpansionGameEngineCore RaindropRenderer)
target_link_libraries(ExpansionGameEngineCore ExpansionMath)
target_link_libraries(ExpansionGameEngineCore BulldozerFileManager)

target_include_directories(ExpansionGameEngineCore PUBLIC ../RaindropRenderer)
target_include_directories(ExpansionGameEngineCore PUBLIC ../ExpansionMath)
target_include_directories(ExpansionGameEngineCore PUBLIC ../BulldozerFileManager)