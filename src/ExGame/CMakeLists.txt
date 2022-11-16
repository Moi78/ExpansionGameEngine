cmake_minimum_required(VERSION 3.0)

project(ExGame)

add_executable(ExGame main.cpp)

target_include_directories(ExGame PUBLIC "../RaindropRenderer/")
target_include_directories(ExGame PUBLIC "../ExpansionGameEngineCore/")
target_include_directories(ExGame PUBLIC "../ExpansionMath/")

target_link_libraries(ExGame RaindropRenderer)
target_link_libraries(ExGame ExpansionGameEngineCore)
target_link_libraries(ExGame ExpansionMath)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

target_link_libraries(ExGame ${JSONCPP_LIBRARIES})
target_include_directories(ExGame PUBLIC ${JSONCPP_INCLUDE_DIRS})

install(TARGETS ExGame RUNTIME DESTINATION bin)
