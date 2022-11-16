#ifndef EXPGE_EXP_MATERIAL_H
#define EXPGE_EXP_MATERIAL_H

#include <fstream>

#include <RD_Material.h>
#include <RD_API.h>
#include <RD_Mesh.h>

#include "EXP_Game.h"

#include "jsoncpp/json/json.h"

class EXP_StaticMesh;

class EXP_Material {
public:
    EXP_Material(EXP_Game* game);
    ~EXP_Material();

    bool LoadMaterial(std::string material_file);
    void RegisterMesh(std::shared_ptr<RD_Mesh> smesh);

private:
    EXP_Game* m_game;
    std::shared_ptr<RD_Material> m_mat;
};


#endif //EXPGE_EXP_MATERIAL_H
