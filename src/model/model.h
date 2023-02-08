#pragma once

#include "../mesh/mesh.h"
#include "../shader/shader_program.h"

#include <assimp/scene.h>
#include <filesystem>
#include <vector>

class Model {
 public:
  Model(const std::filesystem::path &path);
  void draw(ShaderProgram &shader);

 private:
  std::vector<Mesh> meshes;
  std::filesystem::path directory;

  void loadModel(const std::filesystem::path &path);

  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType _type, Texture::Type type);
};