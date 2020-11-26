file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(RaindropRenderer SHARED ${SRCS} ${HEADERS})

target_include_directories(RaindropRenderer PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../libs/GL/include" "stb")
target_link_libraries(RaindropRenderer PUBLIC ExpansionMath BulldozerFileManager)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    target_link_libraries(RaindropRenderer PUBLIC ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
target_include_directories(RaindropRenderer PUBLIC ${GLFW_INCLUDE_DIRS})
target_link_libraries(RaindropRenderer PUBLIC ${GLFW_LIBRARIES})

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    target_include_directories(RaindropRenderer PUBLIC ${GLEW_INCLUDE_DIRS})
    target_link_libraries(RaindropRenderer PUBLIC ${GLEW_LIBRARIES})
endif(GLEW_FOUND)