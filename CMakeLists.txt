cmake_minimum_required(VERSION 3.0)

project(EXPGE)

#FOR DEBUG
#SET(GCC_ARGS "-g3")
#add_definitions(${GCC_ARGS})

set(BUILDTARGET "ubuntu" CACHE STRING "")

if(${BUILDTARGET} STREQUAL "ubuntu")
    set(UBUNTU "-D UBUNTU")
    message("PLATFORM : Linux, Ubuntu")
else()
    set(UBUNTU "")
endif()
add_definitions(${UBUNTU})
add_definitions("-std=c++17")

add_compile_definitions(BUILD_OPENGL)

add_subdirectory(src/ExpansionMath)
add_subdirectory(src/BulldozerFileManager)
add_subdirectory(src/PhysicaSound)
add_subdirectory(src/RaindropRenderer)
add_subdirectory(src/ExpansionGameEngineCore)
add_subdirectory(src/ExGame)
add_subdirectory(src/Game)
