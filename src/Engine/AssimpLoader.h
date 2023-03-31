#ifndef SWTOR_ASSIMPLOADER_H
#define SWTOR_ASSIMPLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <map>

#include "Vertex.h"
#include "../Physics/bounds.h"

struct MeshPack
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    BoundingRegion br;
    aiNode* rootNode;
    std::map<std::string, uint> boneNameToIndexMap;
};

class AssimpLoader
{
public:
    static std::vector<MeshPack> load(std::string path, bool allowArmature = false);
private:
    static std::vector<MeshPack> processNode(aiNode* node, const aiScene* scene, bool allowArmature);
    static MeshPack processMesh(aiMesh* mesh, const aiScene* scene, bool allowArmature);
    static void loadMeshBones(aiMesh* mesh, std::vector<Vertex> &vertices, std::map<std::string, uint> &boneNameToIndexMap);
    static void loadSingleBone(aiMesh* mesh, aiBone* bone, uint boneId, std::vector<Vertex> &vertices);
};

#endif //SWTOR_ASSIMPLOADER_H