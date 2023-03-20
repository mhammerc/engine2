#include "entity_from_model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <entt/entt.hpp>
#include <span>

#include "../common.h"
#include "../graphics/mesh.h"
#include "../graphics/mesh_cache.h"
#include "../graphics/shader_cache.h"
#include "../graphics/texture/texture.h"
#include "../graphics/texture/texture_cache.h"
#include "assimp/material.h"
#include "components/material_component.h"

using namespace engine;

using path = std::filesystem::path;

static auto aimesh_to_mesh(aiMesh const* mesh, path const& name) -> std::unique_ptr<Mesh> {
    std::span<aiVector3D> const raw_vertices(mesh->mVertices, mesh->mNumVertices);
    std::span<aiVector3D> const raw_normals(mesh->mNormals, mesh->mNumVertices);
    // We care only about texture coordinate 0 for now.
    std::span<aiVector3D> const raw_texture_coords(mesh->mTextureCoords[0], mesh->mNumVertices);

    std::vector<Mesh::Vertex> vertices(mesh->mNumVertices);

    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        Mesh::Vertex vertex {};

        glm::vec3 vector;
        vector.x = raw_vertices[i].x;
        vector.y = raw_vertices[i].y;
        vector.z = raw_vertices[i].z;
        vertex.position = vector;

        vector.x = raw_normals[i].x;
        vector.y = raw_normals[i].y;
        vector.z = raw_normals[i].z;
        vertex.normal = vector;

        // does the mesh contain texture coordinates?
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = raw_texture_coords[i].x;
            vec.y = raw_texture_coords[i].y;
            vertex.tex_coords = vec;
        } else {
            vertex.tex_coords = glm::vec2(0.F, 0.F);
        }

        vertices[i] = vertex;
    }

    std::span<aiFace> const raw_faces(mesh->mFaces, mesh->mNumFaces);
    std::vector<unsigned int> indices(raw_faces.size() * 3);

    for (size i = 0; i < raw_faces.size(); ++i) {
        auto face = raw_faces[i];

        std::span<u32> const raw_indices(face.mIndices, face.mNumIndices);
        for (size j = 0; j < raw_indices.size(); ++j) {
            indices[i * 3 + j] = raw_indices[j];
        }
    }

    auto _mesh = std::make_unique<Mesh>(name.string(), std::move(vertices), std::move(indices));

    return _mesh;
}

static auto aimaterial_to_textures(aiMaterial const* mat, aiScene const* /*scene*/, path const& filename)
    -> std::map<i32, std::shared_ptr<Texture>> {
    auto working_directory = path(filename);
    working_directory.remove_filename();

    auto& texture_cache = entt::locator<TextureCache>::value();

    // texture 1 is always diffuse.
    // texture 2 is always specular.
    std::map<i32, std::shared_ptr<Texture>> textures;

    const auto have_diffuse_texture = mat->GetTextureCount(aiTextureType_DIFFUSE);
    if (have_diffuse_texture > 0) {
        aiString filename;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &filename);

        auto texture_path = working_directory / filename.C_Str();

        auto texture = texture_cache.load(
            entt::hashed_string(texture_path.string().c_str()),
            Texture::from_file_2d(texture_path, Texture::Format::SRGB)
        );
        textures.insert({1, texture.first->second.handle()});
    }

    const auto have_specular_texture = mat->GetTextureCount(aiTextureType_SPECULAR);
    if (have_specular_texture > 0) {
        aiString filename;
        mat->GetTexture(aiTextureType_SPECULAR, 0, &filename);

        auto texture_path = working_directory / filename.C_Str();

        auto texture = texture_cache.load(
            entt::hashed_string(texture_path.string().c_str()),
            Texture::from_file_2d(texture_path, Texture::Format::RGB)
        );
        textures.insert({2, texture.first->second.handle()});
    }

    return textures;
}

static auto
on_mesh(entt::entity entity, entt::registry& registry, aiMesh const* aimesh, aiScene const* scene, path const& filename)
    -> void {
    auto& material = registry.get<MaterialComponent>(entity);

    auto& shader_cache = entt::locator<ShaderCache>::value();
    auto& mesh_cache = entt::locator<MeshCache>::value();

    auto _mesh = aimesh_to_mesh(aimesh, filename);
    auto [mesh, _] = mesh_cache.load(entt::hashed_string(filename.string().c_str()), std::move(_mesh));

    material.mesh = mesh->second.handle();
    material.shader = shader_cache["diffuse"_hs].handle();

    if (aimesh->mMaterialIndex >= 0) {
        std::span<aiMaterial*> const raw_materials(scene->mMaterials, scene->mNumMaterials);
        material.textures = aimaterial_to_textures(raw_materials[aimesh->mMaterialIndex], scene, filename);
    }
}

// Each node is a game object.
static auto on_node(
    aiNode const* node,
    aiScene const* scene,
    path const& filename,
    entt::registry& registry,
    std::vector<entt::entity>& entities
) -> void {
    auto entity = entities.emplace_back(registry.create());
    registry.emplace<MaterialComponent>(entity);

    std::span<u32> const raw_mesh_indexes {node->mMeshes, node->mNumMeshes};
    std::span<aiMesh*> const raw_meshes {scene->mMeshes, scene->mNumMeshes};

    // At the moment we support only 1 mesh per assimp node.
    // TODO: a node may have multiple meshes.
    if (!raw_mesh_indexes.empty()) {
        auto* raw_mesh = raw_meshes[0];
        on_mesh(entity, registry, raw_mesh, scene, filename);
    }

    std::span<aiNode*> const node_childrens {node->mChildren, node->mNumChildren};
    for (auto* children : node_childrens) {
        on_node(children, scene, filename, registry, entities);
    }
}

auto engine::entity_from_model(const path& path, entt::registry& registry) -> std::vector<entt::entity> {
    std::vector<entt::entity> entities;

    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(
        path.string().c_str(),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes
            | aiProcess_OptimizeGraph
    );

    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U || scene->mRootNode == nullptr) {
        spdlog::error("Could not load model '{}'. Error: '{}'.", path.string().c_str(), importer.GetErrorString());
        return entities;
    }

    on_node(scene->mRootNode, scene, path, registry, entities);

    return entities;
}