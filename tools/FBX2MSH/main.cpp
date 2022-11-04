#include <iostream>
#include <string>
#include <filesystem>

#include <vec.h>
#include <BD_Writer.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main(int argc, char* argv[]) {
    std::cout << "|----------------------------------------|" << std::endl;
    std::cout << "|                FBX2MSH                 |" << std::endl;
    std::cout << "|----------------------------------------|" << std::endl;

    if(argc <= 1) {
        std::cerr << "ERROR: No file specified." << std::endl;
        return -1;
    }

    std::string filePath(argv[1]);
    if(!std::filesystem::exists(filePath)) {
        std::cerr << "ERROR: " << filePath << " does not exists." << std::endl;
        return -2;
    }

    Assimp::Importer importer;

    std::cout << "Opening " << filePath << std::endl;
    const aiScene* scene = importer.ReadFile(filePath.c_str(),
                                       aiProcess_Triangulate |
                                       aiProcess_GenUVCoords);

    if(!scene) {
        std::cerr << "ERROR: Failed to import " << filePath << std::endl;
        return -4;
    }

    if(!scene->HasMeshes()) {
        std::cerr << "ERROR: There is no mesh in the provided file." << std::endl;
        return -3;
    }

    std::filesystem::path fileDir(filePath);

    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* currentMesh = scene->mMeshes[i];

        if(!currentMesh->HasTextureCoords(0)) {
            std::cerr << "ERROR: This mesh doesn't have any UV Coords." << std::endl;
            continue;
        }

        BD_Writer w;
        for(int v = 0; v < currentMesh->mNumVertices; v++) {
            w.AppendVertex(vec3(currentMesh->mVertices[v].x,
                                currentMesh->mVertices[v].y,
                                currentMesh->mVertices[v].z));

            w.AppendNormal(vec3(currentMesh->mNormals[v].x,
                                currentMesh->mNormals[v].y,
                                currentMesh->mNormals[v].z));

            w.AppendUVcoord(vec2(currentMesh->mTextureCoords[0][v].x,
                                 currentMesh->mTextureCoords[0][v].y));
        }
        std::cout << currentMesh->mNumVertices << " verticies" << std::endl;

        for(int ind = 0; ind < currentMesh->mNumFaces; ind++) {
            for(int idx = 0; idx < currentMesh->mFaces[ind].mNumIndices; idx++) {
                w.AppendIndices(currentMesh->mFaces[ind].mIndices[idx]);
            }
        }
        std::cout << currentMesh->mNumFaces << " faces" << std::endl;

        std::string suffix;
        if(scene->mNumMeshes > 1) {
            suffix = "_" + std::string(currentMesh->mName.C_Str());
        }

        w.ToBinary(std::string(fileDir.parent_path()) + "/", std::string(fileDir.stem()) + suffix);
        std::cout << "Saved " << std::string(fileDir.parent_path()) + "/" << std::string(fileDir.stem()) + suffix << ".msh" << std::endl;
    }

    return 0;
}