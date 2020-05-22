cmake_minimum_required(VERSION 3.0)

project(EXPGE)

#FOR DEBUG
#SET(GCC_ARGS "-g3")
#add_definitions(${GCC_ARGS})

add_subdirectory(src/ExpansionMath)
add_subdirectory(src/BulldozerFileManager)
add_subdirectory(src/PhysicaSound)
add_subdirectory(src/RaindropRenderer)
add_subdirectory(src/ExpansionGameEngineCore)
add_subdirectory(src/ExGame)
add_subdirectory(src/Game)
