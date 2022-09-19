cmake_minimum_required(VERSION 3.0)

project(ExGame)

add_executable(ExGame main.cpp ../ExpansionMath/vec.cpp)

target_include_directories(ExGame PUBLIC "../RaindropRenderer/")
target_include_directories(ExGame PUBLIC "../ExpansionGameEngineCore/")

target_link_libraries(ExGame RaindropRenderer)
target_link_libraries(ExGame ExpansionGameEngineCore)

install(TARGETS ExGame RUNTIME DESTINATION bin)
