cmake_minimum_required(VERSION 3.0)

include_directories(../BulldozerFileManager/ ../ExpansionMath/ ../ExpansionGameEngineCore/ ../RaindropRenderer/ ../PhysicaSound/)

include_directories(../libs/GL/include/)

project(Game)

file(GLOB src "*.cpp" "*.h")

add_library(Game SHARED ${src})

SET(GCC_ARGS "--std=c++17 -rdynamic -fPIC")
add_definitions(${GCC_PERM})

target_link_libraries(Game BulldozerFileManager)
target_link_libraries(Game ExpansionMath)
target_link_libraries(Game ExpansionGameEngineCore)
target_link_libraries(Game RaindropRenderer)
target_link_libraries(Game PhysicaSound)
