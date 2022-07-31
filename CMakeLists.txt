cmake_minimum_required(VERSION 3.7)

project(EXPGE)
add_compile_definitions(BUILD_VULKAN)

add_subdirectory("./RaindropRenderer/")
add_subdirectory("./ExGame/")

install(TARGETS RaindropRenderer RUNTIME DESTINATION bin)
