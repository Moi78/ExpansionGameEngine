file(GLOB SRC "*.cpp")
file(GLOB HEADERS "*.h")

add_library(Game SHARED ${SRC} ${HEADERS})

target_include_directories(Game PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../RaindropRenderer" "../ExpansionGameEngineCore" "../PhysicaSound" "../libs/GL/include")
target_link_libraries(Game PUBLIC ExpansionMath BulldozerFileManager RaindropRenderer PhysicaSound ExpansionGameEngineCore)