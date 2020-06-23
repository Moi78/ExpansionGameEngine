cmake_minimum_required(VERSION 3.0)

include_directories(../BulldozerFileManager/ ../ExpansionMath/ ../ExpansionGameEngineCore/ ../RaindropRenderer/ ../PhysicaSound/)

include_directories(../libs/GL/include/)

project(Game)

file(GLOB src "*.cpp" "*.h")

add_library(Game SHARED ${src})

SET(GCC_ARGS "--std=c++17 -rdynamic -fPIC")
add_definitions(${GCC_PERM})

SET(PREPOC "-D GLM_ENABLE_EXPERIMENTAL")
add_definitions(${PREPOC})

find_package(Bullet REQUIRED)
if(BULLET_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
	target_link_libraries(Game ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY})
endif(BULLET_FOUND)

target_link_libraries(Game BulldozerFileManager)
target_link_libraries(Game ExpansionMath)
target_link_libraries(Game ExpansionGameEngineCore)
target_link_libraries(Game RaindropRenderer)
target_link_libraries(Game PhysicaSound)
