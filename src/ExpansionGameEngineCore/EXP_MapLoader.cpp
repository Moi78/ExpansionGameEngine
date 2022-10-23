#include "EXP_MapLoader.h"

EXP_MapLoader::EXP_MapLoader(EXP_Game* game, std::shared_ptr<EXP_HotLoad> gamelib) {
    m_game = game;
    m_gameLib = gamelib;
}

EXP_MapLoader::~EXP_MapLoader() {

}

std::shared_ptr<EXP_Level> EXP_MapLoader::LoadLevel(std::string levelPath) {
    std::shared_ptr<EXP_Level> voidLevel = std::make_shared<EXP_Level>(m_game);

    std::ifstream lvlFile(levelPath);
    if(!lvlFile.is_open()) {
        std::cerr << "ERROR: Failed to open " << levelPath << std::endl;
        return voidLevel;
    }

    Json::Value root;
    JSONCPP_STRING errs;

    std::cout << "Loading map " << levelPath << std::endl;

    Json::CharReaderBuilder builder;
    builder["collecoComment"] = false;

    if(!Json::parseFromStream(builder, lvlFile, &root, &errs)) {
        std::cerr << "ERROR: Cannot parse " << levelPath << ". " << errs << std::endl;
        return voidLevel;
    }

    std::string levelCodeObjName = root["levelObjectName"].asString() + "Handler";
    LEVELCODEHANDLER hdl = m_gameLib->FetchLibHandler<LEVELCODEHANDLER>(levelCodeObjName.c_str());
    if(hdl == nullptr) {
        std::cerr << "ERROR: Failed to load level code object " << root["levelObjectName"].asString() << std::endl;
        return voidLevel;
    }

    return hdl(m_game);
}