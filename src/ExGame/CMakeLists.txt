cmake_minimum_required(VERSION 3.0)

include_directories(../BulldozerFileManager/ ../ExpansionMath/ ../RaindropRenderer/ ../PhysicaSound/ ../ExpansionGameEngineCore/)

include_directories(../libs/GL/include/)

project(ExGame)

file(GLOB SRC "*.cpp" "*.h")

add_executable(ExGame ${SRC})

SET(GCC_ARGS "--std=c++17 -rdynamic -ldl")
add_definitions(${GCC_ARGS})

SET(PREPOC "-D GLM_ENABLE_EXPERIMENTAL")
add_definitions(${PREPOC})

find_package(Bullet REQUIRED)
if(BULLET_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
	target_link_libraries(ExGame ${BULLET_LIBRARIES})
endif(BULLET_FOUND)

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    include_directories(${OPENAL_INCLUDE_DIRS})
	target_link_libraries(ExGame ${OPENAL_LIBRARIES})
endif(OPENAL_FOUND)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(JSONCPP jsoncpp)
# include_directories("${JSONCPP_INCLUDE_DIRS}/jsoncpp")
# target_link_libraries(ExGame ${JSONCPP_LIBRARIES})

target_link_libraries(ExGame BulldozerFileManager ExpansionMath RaindropRenderer PhysicaSound ExpansionGameEngineCore)
