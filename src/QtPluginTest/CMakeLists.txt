find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)
qt_standard_project_setup()

qt_add_executable(QtPluginTest main.cpp)

target_link_libraries(QtPluginTest PUBLIC Qt::Widgets Qt::Gui)
target_link_libraries(QtPluginTest PUBLIC ExpansionGameEngineCore)

target_link_libraries(QtPluginTest PUBLIC EXPGE_QtPlugin)
target_include_directories(QtPluginTest PUBLIC "../QtPlugin/")