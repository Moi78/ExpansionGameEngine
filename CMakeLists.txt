cmake_minimum_required(VERSION 3.7...3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.16)
endif()

project(EXPGE VERSION 0.1
              DESCRIPTION "Expansion Game Engine"
              LANGUAGES CXX)

set(BUILDTARGET "ubuntu" CACHE STRING "")

if(${BUILDTARGET} STREQUAL "ubuntu")
    set(UBUNTU "-D UBUNTU")
    message("PLATFORM : Linux, Ubuntu")
else()
    set(UBUNTU "")
endif()

add_definitions(${UBUNTU})
add_definitions("-std=c++17")
add_definitions("-fpermissive")
add_definitions("-D_DEBUG")

add_compile_definitions(BUILD_OPENGL)

#file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
#if(EXISTS "${LOC_PATH}")
#    message(FATAL_ERROR "You cannot build in a source directory")
#endif()

add_subdirectory("src/ExpansionMath")
add_subdirectory("src/BulldozerFileManager")
add_subdirectory("src/PhysicaSound")
add_subdirectory("src/RaindropRenderer")
add_subdirectory("src/ExpansionGameEngineCore")
add_subdirectory("src/ExGame")
add_subdirectory("src/Game")

set(CMAKE_INSTALL_PREFIX /usr)
install(
    TARGETS ExpansionGameEngineCore RaindropRenderer ExpansionMath PhysicaSound BulldozerFileManager
    RUNTIME DESTINATION /usr/lib/
)
