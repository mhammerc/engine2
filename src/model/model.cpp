#include "model.h"

#include "spdlog/spdlog.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void Model::draw(ShaderProgram &shader) {
  for (auto &mesh : meshes) {
	mesh.draw(shader);
  }
}

Model::Model(const std::filesystem::path &path) : directory(path) {
  directory.remove_filename();

  loadModel(path);
}

void Model::loadModel(const std::filesystem::path &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
	  path.c_str(),
	  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	spdlog::error("Model: could not load model '{}'.", path.c_str());
	return;
  }

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (size_t i = 0; i < node->mNumMeshes; ++i) {
	auto *mesh = scene->mMeshes[node->mMeshes[i]];
	meshes.emplace_back(std::move(processMesh(mesh, scene)));
  }

  for (size_t i = 0; i < node->mNumChildren; ++i) {
	processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (size_t i = 0; i < mesh->mNumVertices; ++i) {
	Vertex vertex;

	glm::vec3 vector;
	vector.x = mesh->mVertices[i].x;
	vector.y = mesh->mVertices[i].y;
	vector.z = mesh->mVertices[i].z;
	vertex.position = vector;

	vector.x = mesh->mNormals[i].x;
	vector.y = mesh->mNormals[i].y;
	vector.z = mesh->mNormals[i].z;
	vertex.normal = vector;

	if (mesh->mTextureCoords[0])// does the mesh contain texture coordinates?
	{
	  glm::vec2 vec;
	  vec.x = mesh->mTextureCoords[0][i].x;
	  vec.y = mesh->mTextureCoords[0][i].y;
	  vertex.texCoords = vec;
	} else {
	  vertex.texCoords = glm::vec2(0.F, 0.F);
	}

	vertices.push_back(vertex);
  }

  for (size_t i = 0; i < mesh->mNumFaces; ++i) {
	auto face = mesh->mFaces[i];
	for (size_t j = 0; j < face.mNumIndices; ++j) {
	  indices.push_back(face.mIndices[j]);
	}
  }

  if (mesh->mMaterialIndex >= 0) {
	auto *material = scene->mMaterials[mesh->mMaterialIndex];

	{
	  auto diffuseMaps = loadMaterialTextures(material,
											  aiTextureType_DIFFUSE, Texture::Diffuse);
	  textures.insert(textures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
	}

	{
	  std::vector<Texture> specularMaps = loadMaterialTextures(material,
															   aiTextureType_SPECULAR, Texture::Specular);
	  textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
	}
  }

  return Mesh(
	  std::move(vertices),
	  std::move(indices),
	  std::move(textures));
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType _type, Texture::Type type) {
  std::vector<Texture> textures;

  for (size_t i = 0; i < mat->GetTextureCount(_type); ++i) {
	aiString str;
	mat->GetTexture(_type, i, &str);
	auto texture = Texture::from_file(directory / str.C_Str(), type);
	textures.push_back(std::move(texture.value()));
  }

  return textures;
}