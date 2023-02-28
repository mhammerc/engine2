#include "material_component.h"

using namespace engine;

// auto MaterialComponent::shader_update_uniforms() -> void {
//     bool mirror = false;
//     bool glass = false;
//     float explosion = 0.F;

//     shader()->set_uniform("material.shininess", 256.0F);
//     shader()->set_uniform("material.texture_environment", 10);
//     shader()->set_uniform("material.reflect", mirror ? 1.F : 0.F);
//     shader()->set_uniform("material.refract", glass ? 1.F : 0.F);

//     if (_textures.contains(1)) {
//         shader()->set_uniform("material.texture_diffuse1", 1);
//     } else {
//         shader()->set_uniform("material.texture_diffuse1", 0);
//     }

//     if (_textures.contains(2)) {
//         shader()->set_uniform("material.texture_specular1", 2);
//     } else {
//         shader()->set_uniform("material.texture_specular1", 0);
//     }

//     shader()->set_uniform("explosion", explosion);
// }

// auto MaterialComponent::bind() -> void {
//     _shader->bind();

//     for (auto const& texture : _textures) {
//         texture.second->activate_as(texture.first);
//     }
// }

// auto MaterialComponent::unbind() -> void {
//     for (auto const& texture : _textures) {
//         texture.second->activate_as(texture.first, true);
//     }

//     _shader->unbind();
// }

// auto MaterialComponent::shininess() -> float& {
//     return _shininess;
// }

// auto MaterialComponent::reflection() -> float& {
//     return _reflection;
// }

// auto MaterialComponent::refraction() -> float& {
//     return _refraction;
// }

// auto MaterialComponent::textures() -> std::map<i32, std::shared_ptr<Texture>>& {
//     return _textures;
// }

// auto MaterialComponent::shader() -> std::shared_ptr<ShaderProgram>& {
//     return _shader;
// }

// auto MaterialComponent::mesh() -> std::shared_ptr<Mesh>& {
//     return _mesh;
// }
