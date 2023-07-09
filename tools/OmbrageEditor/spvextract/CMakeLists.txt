add_executable(spvextract main.cpp)

target_link_libraries(spvextract jsoncpp OmbrageCompiler)
target_include_directories(spvextract PUBLIC ../compiler/)