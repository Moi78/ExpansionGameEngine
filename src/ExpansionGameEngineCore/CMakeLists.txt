cmake_minimum_required(VERSION 3.22)

include(FetchContent)

add_library(ExpansionGameEngineCore SHARED
            EXP_Game.cpp
            EXP_Game.h
            EXP_Conf.h
            EXP_HotLoad.cpp
            EXP_HotLoad.h
            EXP_MapLoader.cpp
            EXP_MapLoader.h
            EXP_Level.cpp
            EXP_Level.h
            EXP_Actor.cpp
            EXP_Actor.h
            EXP_Component.cpp
            EXP_Component.h
            EXP_Material.cpp
            EXP_Material.h
            EXP_StaticMesh.cpp
            EXP_StaticMesh.h
            EXP_StaticMeshActor.cpp
            EXP_StaticMeshActor.h
            EXP_Camera.cpp
            EXP_Camera.h
            EXP_GenericRessourceManager.h
            EXP_DirLightActor.cpp
            EXP_DirLightActor.h
            EXP_DirLight.cpp
            EXP_DirLight.h
            EXP_PointLight.cpp
            EXP_PointLight.h
            EXP_PointLightActor.cpp
            EXP_PointLightActor.h
            EXP_KeyboardCallback.cpp
            EXP_KeyboardCallback.h
            EXP_MouseCallback.cpp
            EXP_MouseCallback.h
            EXP_InputHandler.cpp
            EXP_InputHandler.h
            EXP_SkeletalMesh.cpp
            EXP_SkeletalMesh.h
            EXP_SkeletalMeshActor.cpp
            EXP_SkeletalMeshActor.h
            EXP_SkelAnim.cpp
            EXP_SkelAnim.h
            EXP_Animator.cpp
            EXP_Animator.h
            EXP_GuiManager.cpp
            EXP_GuiManager.h
            EXP_WidgetBasic.h
            EXP_WidgetBasic.cpp
            ui_kit/EXP_GuiButton.cpp
            ui_kit/EXP_GuiButton.h
            ui_kit/EXP_GuiWindow.cpp
            ui_kit/EXP_GuiWindow.h
            ui_kit/EXP_GuiTextButton.cpp
            EXP_ImGuiClass.h
            EXP_PhysicsHandler.cpp
            EXP_PhysicsHandler.h
            physics/EXP_BoxBody.cpp
            physics/EXP_BoxBody.h
            physics/EXP_Body.cpp
            physics/EXP_Body.h
            physics/EXP_BoxBodyActor.cpp
            physics/EXP_BoxBodyActor.h)

target_link_libraries(ExpansionGameEngineCore RaindropRenderer)
target_link_libraries(ExpansionGameEngineCore ExpansionMath)
target_link_libraries(ExpansionGameEngineCore BulldozerFileManager)

target_include_directories(ExpansionGameEngineCore PUBLIC ../RaindropRenderer)
target_include_directories(ExpansionGameEngineCore PUBLIC ../ExpansionMath)
target_include_directories(ExpansionGameEngineCore PUBLIC ../BulldozerFileManager)
target_include_directories(ExpansionGameEngineCore PUBLIC ./)

set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

FetchContent_Declare(
        jsoncpp
        GIT_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git"
)
FetchContent_MakeAvailable(jsoncpp)

FetchContent_Declare(
        freetype2
        GIT_REPOSITORY "https://github.com/freetype/freetype.git"

)
FetchContent_MakeAvailable(freetype2)

target_link_libraries(ExpansionGameEngineCore jsoncpp)

target_link_libraries(ExpansionGameEngineCore freetype)
target_include_directories(ExpansionGameEngineCore PUBLIC ${freetype2_SOURCE_DIR}/include)

target_include_directories(ExpansionGameEngineCore PUBLIC ${JoltPhysics_SOURCE_DIR}/..)
target_link_libraries(ExpansionGameEngineCore Jolt)