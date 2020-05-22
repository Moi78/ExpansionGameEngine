cmake_minimum_required(VERSION 3.0)

include_directories(../BulldozerFileManager/ ../ExpansionMath/ ../libs/GL/include/)
include_directories(../PhysicaSound/)
include_directories(stb/)

project(RaindropRenderer)

file(GLOB SRC "*.cpp" "*.h")

add_library(RaindropRenderer SHARED ${SRC})

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

SET(PREPOC "-D GLM_ENABLE_EXPERIMENTAL")
add_definitions(${PREPOC})

target_link_libraries(RaindropRenderer BulldozerFileManager)
target_link_libraries(RaindropRenderer ExpansionMath)
target_link_libraries(RaindropRenderer PhysicaSound)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_link_libraries(RaindropRenderer ${OPENGL_LIBRARIES})
endif (OPENGL_FOUND)

find_package(glm REQUIRED)
if(GLM_FOUND)
	include_directories(${GLM_INCLUDE_DIRS})
endif (GLM_FOUND)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(RaindropRenderer ${GLFW_LIBRARIES})

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(RaindropRenderer ${GLEW_LIBRARIES})
endif (GLEW_FOUND)
