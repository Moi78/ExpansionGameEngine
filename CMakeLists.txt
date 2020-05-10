cmake_minimum_required(VERSION 3.0)

project(EXPGE)

#FOR DEBUG
#SET(GCC_ARGS "-g3")
#add_definitions(${GCC_ARGS})

add_subdirectory(ExpansionMath)
add_subdirectory(BulldozerFileManager)
add_subdirectory(PhysicaSound)
add_subdirectory(RaindropRenderer)
add_subdirectory(ExpansionGameEngineCore)
add_subdirectory(Testing)
add_subdirectory(Game)
