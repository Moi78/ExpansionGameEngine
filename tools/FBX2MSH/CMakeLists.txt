cmake_minimum_required(VERSION 3.22)

add_executable(FBX2MSH main.cpp)

target_link_libraries(FBX2MSH ExpansionMath)
target_include_directories(FBX2MSH PUBLIC "../../src/ExpansionMath/")

target_link_libraries(FBX2MSH BulldozerFileManager)
target_include_directories(FBX2MSH PUBLIC "../../src/BulldozerFileManager/")

find_package(assimp REQUIRED)
target_include_directories(FBX2MSH PUBLIC ${assimp_INCLUDE_DIRS})
target_link_libraries(FBX2MSH assimp)