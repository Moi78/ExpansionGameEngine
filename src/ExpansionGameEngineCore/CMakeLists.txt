cmake_minimum_required(VERSION 3.0)

include_directories(../BulldozerFileManager/ ../ExpansionMath/ ../RaindropRenderer/ ../PhysicaSound)

include_directories(../libs/GL/include/)

project(ExpansionGameEngineCore)

file(GLOB SRC "*.cpp" "*.h")

add_library(ExpansionGameEngineCore SHARED ${SRC})

SET(GCC_PERM "-fpermissive --std=c++17 -pthread")
add_definitions(${GCC_PERM})

if(${BUILDTARGET} EQUAL "ubuntu")
    set(UBUNTU "-D UBUNTU")
else()
    set(UBUNTU "")
endif()

SET(PREPOC "-D GLM_ENABLE_EXPERIMENTAL ${UBUNTU}")
add_definitions(${PREPOC})

find_package(Bullet REQUIRED)
if(BULLET_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
	target_link_libraries(ExpansionGameEngineCore ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY})
endif(BULLET_FOUND)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
target_link_libraries(ExpansionGameEngineCore ${JSONCPP_LIBRARIES})
include_directories(${JSONCPP_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ExpansionGameEngineCore Threads::Threads)

target_link_libraries(ExpansionGameEngineCore ExpansionMath)
target_link_libraries(ExpansionGameEngineCore RaindropRenderer)
target_link_libraries(ExpansionGameEngineCore BulldozerFileManager)
