cmake_minimum_required(VERSION 3.7)

project(EXPGE)
add_compile_definitions(BUILD_VULKAN)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("./ExpansionMath/")
add_subdirectory("./RaindropRenderer/")
add_subdirectory("./ExGame/")

install(TARGETS RaindropRenderer RUNTIME DESTINATION bin)
