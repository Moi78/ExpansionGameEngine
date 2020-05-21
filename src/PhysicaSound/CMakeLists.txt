cmake_minimum_required(VERSION 3.0)

include_directories(../ExpansionMath)

project(PhysicaSound)

file(GLOB SRC "*.cpp" "*.h")

add_library(PhysicaSound SHARED ${SRC})

find_package(PkgConfig REQUIRED)

pkg_check_modules(SNDFILE IMPORTED_TARGET sndfile)

target_link_libraries(PhysicaSound PkgConfig::SNDFILE)

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    message("OPENAL FOUND!")

	include_directories(${OPENAL_INCLUDE_DIRS})
	target_link_libraries(PhysicaSound ${OPENAL_LIBRARY})
endif(OPENAL_FOUND)
