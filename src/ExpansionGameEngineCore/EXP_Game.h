#ifndef EXPGE_EXP_GAME_H
#define EXPGE_EXP_GAME_H

#define COLOR_HEX_FLOAT(r, g, b, a) vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)

#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <RaindropRenderer.h>
#include <RD_Skeleton.h>
#include <RD_Callback.h>

#include "EXP_Conf.h"
#include "EXP_GenericRessourceManager.h"
#include "EXP_InputHandler.h"
#include "EXP_GuiManager.h"
#include "EXP_ImGuiClass.h"
#include "EXP_PhysicsHandler.h"

//Forward decl.
class EXP_HotLoad;
class EXP_MapLoader;
class EXP_Level;
class EXP_Material;
class EXP_Animator;
class EXP_Font;

class EXP_Game {
public:
    EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo);
    ~EXP_Game();

    std::string GetGameContentPath();
    std::string GetEngineContentPath();

    // CLI Arguments
    void GameArguments(int argc, char* argv[]);
    std::vector<std::string> GetGameArguments();

    //Engine control
    bool InitEngine();
    void RunGame();
    void LoadLevel(std::string levelPath);

    //Engine Sub-Systems
    std::shared_ptr<RaindropRenderer> GetRenderer();
    std::shared_ptr<EXP_InputHandler> GetInputHandler();
    std::shared_ptr<EXP_Animator> GetAnimator();
    std::shared_ptr<EXP_GuiManager> GetGuiManager();
    std::shared_ptr<EXP_Level> GetCurrentLevel();
    std::shared_ptr<EXP_PhysicsHandler> GetPhysicsHandler();

    std::shared_ptr<EXP_Material> QueryMaterial(
            std::string matPath,
            bool fromEngine = false,
            bool no_cache = false,
            bool autobuild = true
    );

    int GetSkeletonOffset();
    void SetupSkeleton(std::shared_ptr<RD_Skeleton> skel);
    void UpdateSkeleton(std::shared_ptr<RD_Skeleton> skel);

    void Resize();

    std::shared_ptr<EXP_Font> GetEngineFont();

    void AddImGuiClass(std::shared_ptr<EXP_ImGuiClass> gui_class);

    float& GetDeltaTime();
private:
    std::shared_ptr<RaindropRenderer> m_rndr;
    float m_deltaTime;

    std::shared_ptr<EXP_HotLoad> m_hotloader;
    std::unique_ptr<EXP_MapLoader> m_maploader;
    std::shared_ptr<EXP_InputHandler> m_inhdl;
    std::shared_ptr<EXP_Animator> m_animator;
    std::shared_ptr<EXP_PhysicsHandler> m_physics_hdl;

    std::shared_ptr<EXP_GuiManager> m_guiLayer;
    std::shared_ptr<EXP_Font> m_defaultFont;

    std::shared_ptr<EXP_Level> m_currentLevel;

    EXP_GameInfo m_gameinfo;
    EXP_GenericRessourceManager<std::shared_ptr<EXP_Material>> m_materials;

    std::vector<std::shared_ptr<EXP_ImGuiClass>> m_imgui_classes;

    int m_last_skel_offset;

    std::vector<std::string> m_gameArguments;
};


#endif //EXPGE_EXP_GAME_H
