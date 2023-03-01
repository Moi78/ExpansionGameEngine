#include <iostream>
#include <string>
#include <filesystem>

#include <vec.h>
#include <BD_Writer.h>
#include <BD_SkelWriter.h>
#include <BD_AnimWriter.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>

int FindBoneIdx(std::string name, std::vector<Bone>& bones) {
    int i = 0;
    for(auto& b : bones) {
        if(name == b.name) {
            return i;
        }

        i++;
    }

    return -1;
}

void ResolveBonesHierarchy(std::vector<Bone>& bones) {
    for(auto& b : bones) {
        b.parent_id = FindBoneIdx(b.parent_name, bones);
    }
}

std::vector<BD_Frame> ExtractChannelFrames(aiNodeAnim* anim, std::vector<Bone>& bones) {
    std::string nodeName = anim->mNodeName.C_Str();
    int nodeIdx = FindBoneIdx(nodeName, bones);
    if(nodeIdx == -1) {
        std::cerr << "ERROR: Bad node id." << std::endl;
        return {};
    }

    std::vector<BD_Frame> frames;
    for(int i = 0; i < anim->mNumPositionKeys; i++) {
        BD_Frame f;
        f.boneID = nodeIdx;
        f.chanType = BD_CHAN_POS;

        f.time = anim->mPositionKeys[i].mTime;

        for(int c = 0; c < 3; c++) {
            f.vecData[c] = anim->mPositionKeys[i].mValue[c];
        }
        f.vecData[3] = 0;

        frames.push_back(f);
    }

    for(int i = 0; i < anim->mNumRotationKeys; i++) {
        BD_Frame f;
        f.boneID = nodeIdx;
        f.chanType = BD_CHAN_ROT;

        f.time = anim->mRotationKeys[i].mTime;

        f.vecData[0] = anim->mRotationKeys[i].mValue.x;
        f.vecData[1] = anim->mRotationKeys[i].mValue.y;
        f.vecData[2] = anim->mRotationKeys[i].mValue.z;
        f.vecData[3] = anim->mRotationKeys[i].mValue.w;

        frames.push_back(f);
    }

    for(int i = 0; i < anim->mNumScalingKeys; i++) {
        BD_Frame f;
        f.boneID = nodeIdx;
        f.chanType = BD_CHAN_SCALE;

        f.time = anim->mPositionKeys[i].mTime;

        for(int c = 0; c < 3; c++) {
            f.vecData[c] = anim->mScalingKeys[i].mValue[c];
        }
        f.vecData[3] = 0;

        frames.push_back(f);
    }

    return frames;
}

int main(int argc, char* argv[]) {
    std::cout << "|----------------------------------------|" << std::endl;
    std::cout << "|                FBX2MSH                 |" << std::endl;
    std::cout << "|----------------------------------------|" << std::endl;

    std::string filePath;
    std::string outpFilePath;
    bool doExtractSkeleton = false;
    bool doExtractAnims = false;

    for(int i = 0; i < argc; i++) {
        if(std::string(argv[i]) == "-f") {
            if(i + 1 >= argc) {
                std::cerr << "ERROR: No file specified" << std::endl;
                return -1;
            }

            filePath = std::string(argv[i + 1]);
            if(!std::filesystem::exists(filePath)) {
                std::cerr << "ERROR: " << filePath << " does not exists." << std::endl;
                return -2;
            }

        } else if(std::string(argv[i]) == "-extract-skeleton") {
            doExtractSkeleton = true;

        } else if(std::string(argv[i]) == "-o") {
            if(i + 1 >= argc) {
                std::cerr << "ERROR: No output path specified" << std::endl;
                return -1;
            }

            outpFilePath = std::string(argv[i + 1]);
        } else if(std::string(argv[i]) == "-extract-anims") {
            doExtractAnims = true;
        }
    }

    if(argc <= 1) {
        std::cout << "Usage: fbx2msh -f input_file [-extract-skeleton]" << std::endl;
        return 0;
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
    std::filesystem::path outpDir(outpFilePath);

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
        std::cout << currentMesh->mNumVertices << " vertices" << std::endl;

        for(int ind = 0; ind < currentMesh->mNumFaces; ind++) {
            for(int idx = 0; idx < currentMesh->mFaces[ind].mNumIndices; idx++) {
                w.AppendIndices(currentMesh->mFaces[ind].mIndices[idx]);
            }
        }
        std::cout << currentMesh->mNumFaces << " faces" << std::endl;

        std::vector<Bone> bones;
        if(doExtractSkeleton) {
            std::vector<vec4> weights(currentMesh->mNumVertices);
            for(auto& wgt : weights) {
                wgt = vec4(0, 0, 0, 0);
            }

            std::vector<vec4> bonesID(currentMesh->mNumVertices);
            for(auto& b : bonesID) {
                b = vec4(-1, -1, -1, -1);
            }

            if(currentMesh->HasBones()) {
                BD_SkelWriter sk_w;

                for(int b = 0; b < currentMesh->mNumBones; b++) {
                    unsigned int nWeights = currentMesh->mBones[b]->mNumWeights;
                    for(int wgt = 0; wgt < nWeights; wgt++) {
                        //Determining where to put the weighting
                        int idx = 0;
                        for(int a = 0; a < 4; a++) {
                            if(bonesID[currentMesh->mBones[b]->mWeights[wgt].mVertexId][a] == -1) {
                                idx = a;
                                break;
                            }
                        }

                        weights[currentMesh->mBones[b]->mWeights[wgt].mVertexId][idx] = currentMesh->mBones[b]->mWeights[wgt].mWeight;
                        bonesID[currentMesh->mBones[b]->mWeights[wgt].mVertexId][idx] = b;
                    }

                    Bone bn;
                    bn.name = currentMesh->mBones[b]->mName.C_Str();
                    bn.idx = b;

                    aiMatrix4x4t mat = currentMesh->mBones[b]->mOffsetMatrix;
                    bn.offset = mat4f(mat[0]);

                    aiNode* root = scene->mRootNode;
                    aiNode* currentBone = root->FindNode(bn.name.c_str());

                    bn.parent_name = currentBone->mParent->mName.C_Str();
                    bn.pos = mat4f(currentBone->mTransformation[0]);

                    bones.push_back(bn);
                }

                ResolveBonesHierarchy(bones);

                for(auto& wgt : weights) {
                    w.AppendVertexWeight(wgt);
                }

                for(auto& b : bonesID) {
                    w.AppendBoneID(b);
                }

                for(auto& b : bones) {
                    sk_w.AppendBone(b);
                }

                std::filesystem::path outp_skel(outpDir.parent_path());
                sk_w.WriteBinary(outp_skel.string() + "/" + outpDir.stem().string() + ".skl");

                std::cout << currentMesh->mNumBones << " bones" << std::endl;
            } else {
                std::cerr << "ERROR: No bones detected." << std::endl;
            }
        }

        if(doExtractAnims) {
            if(!scene->HasAnimations()) {
                std::cerr << "ERROR: No animations detected." << std::endl;
            } else {
                for(int a = 0; a < scene->mNumAnimations; a++) {
                    aiAnimation* anim = scene->mAnimations[a];
                    std::cout << "Anim " << a << " duration : " << anim->mDuration / anim->mTicksPerSecond << " sec" << std::endl;
                    std::cout << "Framerate : " << anim->mTicksPerSecond << " FPS" << std::endl;

                    BD_AnimMeta meta{};
                    meta.framerate = anim->mTicksPerSecond;
                    meta.duration = anim->mDuration;

                    std::filesystem::path outp_anim(outpDir.parent_path());
                    BD_AnimWriter aw;
                    for(int chan = 0; chan < anim->mNumChannels; chan++) {
                        assert(bones.size() > 0);
                        auto animData = ExtractChannelFrames(anim->mChannels[chan], bones);

                        std::cout << "Channel " << chan << " frame count : " << animData.size() << std::endl;
                        for(auto f : animData) {
                            aw.AddFrame(f);
                        }
                    }

                    aw.SetAnimMeta(meta);
                    aw.WriteAnimation(outp_anim.string() + "/" + outpDir.stem().string() + ".anim");
                }
            }
        }

        std::string suffix;
        if(scene->mNumMeshes > 1) {
            suffix = "_" + std::string(currentMesh->mName.C_Str());
        }

        if(outpFilePath.empty()) {
            w.ToBinary(std::string(fileDir.parent_path()) + "/" + std::string(fileDir.stem()) + suffix + ".msh");
            std::cout << "Saved " << std::string(fileDir.parent_path()) + "/" << std::string(fileDir.stem()) + suffix
                      << ".msh" << std::endl;
        } else {
            std::string opath = outpDir.parent_path().string() + "/" + outpDir.stem().string() + suffix + ".msh";

            w.ToBinary(opath);
            std::cout << "Saved " << opath << std::endl;
        }
    }

    return 0;
}