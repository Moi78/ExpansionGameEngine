cmake_minimum_required(VERSION 3.22)

add_library(Game SHARED GameLevel.cpp GameLevel.h TestActor.cpp TestActor.h
            PhysicsCube.cpp
            PhysicsCube.h)

target_include_directories(Game PUBLIC "../RaindropRenderer/")
target_include_directories(Game PUBLIC "../ExpansionGameEngineCore/")

target_link_libraries(Game RaindropRenderer)
target_link_libraries(Game ExpansionGameEngineCore)

add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Game> ${PROJECT_SOURCE_DIR}/src/GameDir/)