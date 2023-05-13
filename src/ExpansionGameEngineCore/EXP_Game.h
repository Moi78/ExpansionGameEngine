#ifndef EXPGE_EXP_GAME_H
#define EXPGE_EXP_GAME_H

#include <memory>
#include <string>

#include <RaindropRenderer.h>
#include <RD_Skeleton.h>
#include <RD_Callback.h>

#include "EXP_Conf.h"
#include "EXP_GenericRessourceManager.h"
#include "EXP_InputHandler.h"
#include "EXP_GuiManager.h"

//Forward decl.
class EXP_HotLoad;
class EXP_MapLoader;
class EXP_Level;
class EXP_Material;
class EXP_Animator;

class EXP_Game {
public:
    EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo);
    ~EXP_Game();

    std::string GetGameContentPath();
    std::string GetEngineContentPath();

    //Engine control
    bool InitEngine();
    void RunGame();
    void LoadLevel(std::string levelPath);

    //Engine Sub-Systems
    std::shared_ptr<RaindropRenderer> GetRenderer();
    std::shared_ptr<EXP_InputHandler> GetInputHandler();
    std::shared_ptr<EXP_Animator> GetAnimator();
    std::shared_ptr<EXP_GuiManager> GetGuiManager();

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

private:
    std::shared_ptr<RaindropRenderer> m_rndr;

    std::shared_ptr<EXP_HotLoad> m_hotloader;
    std::unique_ptr<EXP_MapLoader> m_maploader;
    std::shared_ptr<EXP_InputHandler> m_inhdl;
    std::shared_ptr<EXP_Animator> m_animator;

    std::shared_ptr<EXP_GuiManager> m_guiLayer;

    std::shared_ptr<EXP_Level> m_currentLevel;

    EXP_GameInfo m_gameinfo;
    EXP_GenericRessourceManager<std::shared_ptr<EXP_Material>> m_materials;

    int m_last_skel_offset;
};


#endif //EXPGE_EXP_GAME_H
