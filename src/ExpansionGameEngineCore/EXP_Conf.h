#ifndef EXPGE_EXP_CONF_H
#define EXPGE_EXP_CONF_H

#include <string>

struct EXP_GameInfo {
    int WindowWidth;
    int WindowHeight;

    std::string GameLib;
    std::string RootGameContentDir;
    std::string RootEngineContentDir;

    std::string StartLevel;
};

#endif //EXPGE_EXP_CONF_H
