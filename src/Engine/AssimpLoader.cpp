#include "AssimpLoader.h"

std::vector<MeshPack> AssimpLoader::load(std::string path) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::exception();
    }
    return processNode(scene->mRootNode, scene);
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