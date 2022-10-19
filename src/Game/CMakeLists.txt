cmake_minimum_required(VERSION 3.22)

add_library(Game SHARED GameLevel.cpp GameLevel.h)

target_include_directories(ExGame PUBLIC "../RaindropRenderer/")
target_include_directories(ExGame PUBLIC "../ExpansionGameEngineCore/")

target_link_libraries(ExGame RaindropRenderer)
target_link_libraries(ExGame ExpansionGameEngineCore)

add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Game> ${PROJECT_SOURCE_DIR}/src/GameDir/)