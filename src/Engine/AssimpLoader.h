#ifndef SWTOR_ASSIMPLOADER_H
#define SWTOR_ASSIMPLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Vertex.h"
#include "../Physics/bounds.h"

struct MeshPack
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    BoundingRegion br;
};

class AssimpLoader
{
public:
    static std::vector<MeshPack> load(std::string path);
private:
    static std::vector<MeshPack> processNode(aiNode* node, const aiScene* scene);
    static MeshPack processMesh(aiMesh* mesh, const aiScene* scene);
};



#endif //SWTOR_ASSIMPLOADER_H