add_executable(ExGame "main.cpp")

target_include_directories(ExGame PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../RaindropRenderer" "../ExpansionGameEngineCore" "../PhysicaSound" "../libs/GL/include")

add_definitions("-rdynamic -ldl")

target_link_libraries(ExGame PUBLIC BulldozerFileManager ExpansionMath RaindropRenderer PhysicaSound ExpansionGameEngineCore)