cmake_minimum_required(VERSION 3.22)

add_library(ExpansionGameEngineCore SHARED
        EXP_Game.cpp EXP_Game.h EXP_Conf.h EXP_HotLoad.cpp EXP_HotLoad.h EXP_MapLoader.cpp EXP_MapLoader.h EXP_Level.cpp EXP_Level.h EXP_Actor.cpp EXP_Actor.h EXP_Component.cpp EXP_Component.h EXP_Material.cpp EXP_Material.h EXP_StaticMesh.cpp EXP_StaticMesh.h EXP_StaticMeshActor.cpp EXP_StaticMeshActor.h EXP_Camera.cpp EXP_Camera.h EXP_GenericRessourceManager.h EXP_DirLightActor.cpp EXP_DirLightActor.h EXP_DirLight.cpp EXP_DirLight.h EXP_PointLight.cpp EXP_PointLight.h EXP_PointLightActor.cpp EXP_PointLightActor.h)

target_link_libraries(ExpansionGameEngineCore RaindropRenderer)
target_link_libraries(ExpansionGameEngineCore ExpansionMath)
target_link_libraries(ExpansionGameEngineCore BulldozerFileManager)

target_include_directories(ExpansionGameEngineCore PUBLIC ../RaindropRenderer)
target_include_directories(ExpansionGameEngineCore PUBLIC ../ExpansionMath)
target_include_directories(ExpansionGameEngineCore PUBLIC ../BulldozerFileManager)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

target_link_libraries(ExpansionGameEngineCore ${JSONCPP_LIBRARIES})
target_include_directories(ExpansionGameEngineCore PUBLIC ${JSONCPP_INCLUDE_DIRS})