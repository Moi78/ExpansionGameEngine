#ifndef EXPGE_OMBRAGEMAINLEVEL_H
#define EXPGE_OMBRAGEMAINLEVEL_H

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>

#include <EXP_Camera.h>

#include "UI.h"
#include "OmbrageCamera.h"

class OmbrageMainLevel : public EXP_Level {
public:
    OmbrageMainLevel(EXP_Game* game);
    ~OmbrageMainLevel();

    void OnStart() override;

private:
    std::shared_ptr<OmbrageUI::UI> m_ui;
    std::shared_ptr<OmbrageUI::OmbrageCamera> m_cam;
};

EXPGE_LEVEL_HEADER(OmbrageMainLevel);

#endif //EXPGE_OMBRAGEMAINLEVEL_H
