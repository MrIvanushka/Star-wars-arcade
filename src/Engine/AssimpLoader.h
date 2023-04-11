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
#include "../BaseComponents/Animator.h"

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
    static std::vector<MeshPack> load(Assimp::Importer &importer, std::string path);
    static std::vector<MeshPack> loadWithArmature(Assimp::Importer &importer, std::string path, std::vector<AnimationClip> &clips);
private:
    static std::vector<MeshPack> processNode(aiNode* node, const aiScene* scene);
    static MeshPack processMesh(aiMesh* mesh, const aiScene* scene);
    static std::vector<MeshPack> processNodeWithArmature(aiNode* node, const aiScene* scene, std::map<std::string, glm::mat4> &boneOffsets);
    static MeshPack processMeshWithArmature(aiMesh* mesh, const aiScene* scene, std::map<std::string, glm::mat4> &boneOffsets);
    static void loadMeshBones(aiMesh* mesh, MeshPack &ret, std::map<std::string, glm::mat4> &boneOffsets);
    static void loadSingleBone(aiMesh* mesh, aiBone* bone, uint boneId, std::vector<Vertex> &vertices);
};

#endif //SWTOR_ASSIMPLOADER_H