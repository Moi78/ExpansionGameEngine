find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)
qt_standard_project_setup()

qt_add_library(EXPGE_QtPlugin SHARED EXPGE_Widget.cpp EXPGE_Widget.h)

target_link_libraries(EXPGE_QtPlugin Qt::Widgets Qt::Gui)
target_link_libraries(EXPGE_QtPlugin ExpansionGameEngineCore)
