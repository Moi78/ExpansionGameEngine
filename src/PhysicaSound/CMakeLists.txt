file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(PhysicaSound SHARED ${SRCS} ${HEADERS})

target_include_directories(PhysicaSound PUBLIC "../ExpansionMath" "../BulldozerFileManager")
target_link_libraries(PhysicaSound PUBLIC ExpansionMath BulldozerFileManager)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE sndfile)
target_include_directories(PhysicaSound PUBLIC ${SNDFILE_INCLUDE_DIRS})
target_link_libraries(PhysicaSound PUBLIC ${SNDFILE_LIBRARIES})

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    target_include_directories(PhysicaSound PUBLIC ${OPENAL_INCLUDE_DIRS})
    target_link_libraries(PhysicaSound PUBLIC ${OPENAL_LIBRARY})
endif(OPENAL_FOUND)