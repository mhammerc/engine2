#pragma once

#include <filesystem>
#include <vector>

#include "assimp/scene.h"
#include "mesh.h"
#include "shader_program.h"

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

    auto loadMaterialTextures(aiMaterial* mat, aiTextureType _type, Texture::Type type)
        -> std::vector<std::unique_ptr<Texture>>;
};