#include "AssimpLoader.h"
#include"../Utilities/AssimpMath.h"

std::vector<MeshPack> AssimpLoader::load(Assimp::Importer &importer, std::string path) {
    //Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::exception();
    }
    auto ret = processNode(scene->mRootNode, scene);
    
    return ret;
}

std::vector<MeshPack> AssimpLoader::loadWithArmature(Assimp::Importer &importer, std::string path, std::vector<AnimationClip> &clips)
{
    //Assimp::Importer importer;
    std::map<std::string, glm::mat4> boneOffsets;
    
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::exception();
    }
    auto ret = processNodeWithArmature(scene->mRootNode, scene, boneOffsets);

    glm::mat4 globalInverse;
    AssimpMath::convert_aimatrix_to_glm(globalInverse, scene->mRootNode->mTransformation);

    for(auto i = 0u; i<scene->mNumAnimations; i++){
        clips.push_back(AnimationClip(scene->mAnimations[i], scene->mRootNode, boneOffsets, globalInverse));
    }

    return ret;
}

std::vector<MeshPack> AssimpLoader::processNode(aiNode* node, const aiScene* scene) {
    std::vector<MeshPack> meshes;
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshPack newMesh = processMesh(mesh, scene);
        meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::vector<MeshPack> childrenMeshes = processNode(node->mChildren[i], scene);
        meshes.insert(meshes.end(), childrenMeshes.begin(), childrenMeshes.end());
    }

    return meshes;
}

// process mesh in object file
MeshPack AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
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
    
    ret.vertices = vertices;
    ret.indices = indices;
    ret.br = br;
    return ret;
}

std::vector<MeshPack> AssimpLoader::processNodeWithArmature(aiNode* node, const aiScene* scene, std::map<std::string, glm::mat4> &boneOffsets) {
    std::vector<MeshPack> meshes;
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshPack newMesh = processMeshWithArmature(mesh, scene, boneOffsets);
        meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::vector<MeshPack> childrenMeshes = processNodeWithArmature(node->mChildren[i], scene, boneOffsets);
        meshes.insert(meshes.end(), childrenMeshes.begin(), childrenMeshes.end());
    }
    glm::mat4 globalInverse;
     AssimpMath::convert_aimatrix_to_glm(globalInverse, node->mTransformation);

    return meshes;
}

// process mesh in object file
MeshPack AssimpLoader::processMeshWithArmature(aiMesh* mesh, const aiScene* scene, std::map<std::string, glm::mat4> &boneOffsets) {
    MeshPack ret = processMesh(mesh, scene);

    loadMeshBones(mesh, ret, boneOffsets);
    
    return ret;
}

void AssimpLoader::loadMeshBones(aiMesh* mesh, MeshPack &ret, std::map<std::string, glm::mat4> &boneOffsets)
{
    for (uint i = 0 ; i < mesh->mNumBones; i++) 
    {
        loadSingleBone(mesh, mesh->mBones[i], i, ret.vertices);
        ret.boneNameToIndexMap[mesh->mBones[i]->mName.C_Str()] = i;
        boneOffsets[mesh->mBones[i]->mName.C_Str()] = glm::mat4(1.0f);
        AssimpMath::convert_aimatrix_to_glm(boneOffsets[mesh->mBones[i]->mName.C_Str()], mesh->mBones[i]->mOffsetMatrix);
    }
}

void AssimpLoader::loadSingleBone(aiMesh* mesh, aiBone* bone, uint boneId, std::vector<Vertex> &vertices)
{
    for (uint i = 0 ; i < bone->mNumWeights ; i++) {
        const aiVertexWeight& vw = bone->mWeights[i];
        //might cause errors with vertex id
        
        for(uint j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++)
        {

            if(vertices[bone->mWeights[i].mVertexId].boneWeights[j] < 0.001)
            {
                vertices[bone->mWeights[i].mVertexId].boneWeights[j] = vw.mWeight;
                vertices[bone->mWeights[i].mVertexId].boneIds[j] = boneId;
                break;
            }
        }
    }
}