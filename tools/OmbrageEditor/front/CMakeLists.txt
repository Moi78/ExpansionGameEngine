cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)

add_executable(OmbrageEditor main.cpp)
add_library(OmbrageLib SHARED lib/OmbrageMainLevel.cpp lib/OmbrageMainLevel.h lib/UI.cpp lib/UI.h lib/OmbrageCamera.cpp lib/OmbrageCamera.h lib/NodeGraph_UI.cpp lib/NodeGraph_UI.h lib/Node.h lib/ShaderNode.cpp lib/ShaderNode.h lib/Node.cpp lib/MathNodes.cpp lib/MathNodes.h lib/SplitNode.cpp lib/SplitNode.h)

find_package(fmt REQUIRED)

target_link_libraries(OmbrageEditor BulldozerFileManager ExpansionGameEngineCore ExpansionMath RaindropRenderer OmbrageCompiler fmt)
target_link_libraries(OmbrageLib BulldozerFileManager ExpansionGameEngineCore ExpansionMath RaindropRenderer OmbrageCompiler fmt)

target_include_directories(OmbrageEditor PUBLIC "../compiler/")
target_include_directories(OmbrageLib PUBLIC "../compiler/")

add_custom_command(TARGET OmbrageLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:OmbrageLib> ${PROJECT_SOURCE_DIR}/tools/OmbrageEditor/front/ombrage_content/lib/)