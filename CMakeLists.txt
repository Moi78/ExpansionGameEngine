cmake_minimum_required(VERSION 3.7)

project(EXPGE)
add_compile_definitions(BUILD_VULKAN)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("./src/ExpansionMath/")
add_subdirectory("./src/BulldozerFileManager/")
add_subdirectory("./src/RaindropRenderer/")
add_subdirectory("./src/ExpansionGameEngineCore/")
add_subdirectory("./src/ExGame/")
add_subdirectory("./src/Game/")

add_subdirectory("./src/QtPlugin/")
add_subdirectory("./src/QtPluginTest/")

add_subdirectory("./tools/FBX2MSH/")
add_subdirectory("./tools/OmbrageEditor/front")

install(TARGETS ExGame RUNTIME DESTINATION bin)
