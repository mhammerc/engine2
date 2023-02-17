#pragma once

#include <assimp/scene.h>

#include <filesystem>
#include <vector>

#include "../shader/shader_program.h"
#include "mesh.h"

class Model {
  public:
    Model(const std::filesystem::path& path);
    void draw(ShaderProgram& shader);

  private:
    std::vector<Mesh> meshes;
    std::filesystem::path directory;

    void loadModel(const std::filesystem::path& path);

    void processNode(aiNode* node, const aiScene* scene);
    auto processMesh(aiMesh* mesh, const aiScene* scene) -> Mesh;

    auto loadMaterialTextures(aiMaterial* mat, aiTextureType _type, Texture::Type type) -> std::vector<Texture>;
};