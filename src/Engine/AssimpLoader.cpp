#include "AssimpLoader.h"

std::vector<MeshPack> AssimpLoader::load(std::string path, bool allowArmature) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::exception();
    }
    auto ret = processNode(scene->mRootNode, scene, allowArmature);

    for (auto& element : ret)
        element.rootNode = scene->mRootNode;
    
    return ret;
}

std::vector<MeshPack> AssimpLoader::processNode(aiNode* node, const aiScene* scene, bool allowArmature) {
    std::vector<MeshPack> meshes;
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshPack newMesh = processMesh(mesh, scene, allowArmature);
        meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::vector<MeshPack> childrenMeshes = processNode(node->mChildren[i], scene, allowArmature);
        meshes.insert(meshes.end(), childrenMeshes.begin(), childrenMeshes.end());
    }

    return meshes;
}

// process mesh in object file
MeshPack AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene, bool allowArmature) {
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<unsigned int> indices(3 * mesh->mNumFaces);
    // setup bounding region
    BoundingRegion br(BoundTypes::SPHERE);
    glm::vec3 min(std::numeric_limits<float>::max()); // min point = max float
    glm::vec3 max(std::numeric_limits<float>::min()); // max point = min float
    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // position
        vertices[i].position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        // determine if outside of current min and max
        for (int j = 0; j < 3; j++) {
            // if smaller than min
            if (vertices[i].position[j] < min[j]) min[j] = vertices[i].position[j];
            // if larger than max
            if (vertices[i].position[j] > max[j]) max[j] = vertices[i].position[j];
        }
        // normal vectors
        vertices[i].normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );
        // textures
        if (mesh->mTextureCoords[0]) {
            vertices[i].texcoord = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertices[i].texcoord = glm::vec2(0.0f);
        }
    }
    // process min/max for BR
    // calculate max distance from the center
    br.center = (min + max) / 2.0f;
    br.ogCenter = br.center;
    br.collider = NULL;
    float maxRadiusSquared = 0.0f;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        float radiusSquared = 0.0f; // distance for this vertex
        for (int j = 0; j < 3; j++) {
            radiusSquared += (vertices[i].position[j] - br.center[j]) * (vertices[i].position[j] - br.center[j]);
        }
        if (radiusSquared > maxRadiusSquared) {
            // found new squared radius
            // a^2 > b^2 --> |a| > |b|
            maxRadiusSquared = radiusSquared;
        }
    }

    br.radius = sqrt(maxRadiusSquared);
    br.ogRadius = br.radius;

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    MeshPack ret;

    if(allowArmature)
    {
        loadMeshBones(mesh, vertices, ret.boneNameToIndexMap);
    }
    
    ret.vertices = vertices;
    ret.indices = indices;
    ret.br = br;
    return ret;
}

void AssimpLoader::loadMeshBones(aiMesh* mesh, std::vector<Vertex> &vertices, std::map<std::string, uint> &boneNameToIndexMap)
{
    for (uint i = 0 ; i < mesh->mNumBones ; i++) 
    {
        loadSingleBone(mesh, mesh->mBones[i], i, vertices);
        boneNameToIndexMap[mesh->mBones[i]->mName.C_Str()] = i;
    }
}

void AssimpLoader::loadSingleBone(aiMesh* mesh, aiBone* bone, uint boneId, std::vector<Vertex> &vertices)
{
    for (uint i = 0 ; i < bone->mNumWeights ; i++) {
        const aiVertexWeight& vw = bone->mWeights[i];
        //might cause errors with vertex id
        for(uint j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++)
        {
            if(vertices[bone->mWeights[i].mVertexId].bones.Weights[j] < 0.001)
            {
                vertices[bone->mWeights[i].mVertexId].bones.Weights[i] = vw.mWeight;
                vertices[bone->mWeights[i].mVertexId].bones.Ids[i] = boneId;
                break;
            }
        }
    }
}