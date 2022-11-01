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

    auto lvl = hdl(m_game);

    Json::Value nodes = root["nodes"];
    for(auto& n : nodes) {
        if(n["type"].asString() == "mesh") {
            lvl->RegisterActor(CreateMesh(n));
        } else if(n["type"].asString() == "dirlight") {
            lvl->RegisterActor(CreateDirLight(n));
        } else if(n["type"].asString() == "pointlight") {
            lvl->RegisterActor(CreatePointLight(n));
        }
    }

    return lvl;
}

std::shared_ptr<EXP_StaticMeshActor> EXP_MapLoader::CreateMesh(Json::Value node) {
    vec3 pos = vec3(node["pos"][0].asFloat(), node["pos"][1].asFloat(), node["pos"][2].asFloat());
    vec3 rot = vec3(node["rot"][0].asFloat(), node["rot"][1].asFloat(), node["rot"][2].asFloat());
    vec3 scale = vec3(node["scale"][0].asFloat(), node["scale"][1].asFloat(), node["scale"][2].asFloat());

    std::shared_ptr<EXP_StaticMeshActor> smesh = std::make_shared<EXP_StaticMeshActor>(
            m_game, node["mesh"].asString(), m_game->QueryMaterial(node["material"].asString()), pos, rot, scale
    );

    return smesh;
}

std::shared_ptr<EXP_DirLightActor> EXP_MapLoader::CreateDirLight(Json::Value node) {
    vec3 color = vec3(node["color"][0].asFloat(), node["color"][1].asFloat(), node["color"][2].asFloat());
    vec3 dir = vec3(node["dir"][0].asFloat(), node["dir"][1].asFloat(), node["dir"][2].asFloat());
    float brightness = node["brightness"].asFloat();

    return std::make_shared<EXP_DirLightActor>(m_game, dir, color, brightness);
}

std::shared_ptr<EXP_PointLightActor> EXP_MapLoader::CreatePointLight(Json::Value node) {
    vec3 pos = vec3(node["pos"][0].asFloat(), node["pos"][1].asFloat(), node["pos"][2].asFloat());
    vec3 color = vec3(node["color"][0].asFloat(), node["color"][1].asFloat(), node["color"][2].asFloat());

    float brightness = node["brightness"].asFloat();
    float radius = node["radius"].asFloat();

    return std::make_shared<EXP_PointLightActor>(m_game, pos, color, brightness, radius);
}