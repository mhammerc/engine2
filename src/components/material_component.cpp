#include "material_component.h"

#include "entt/entt.hpp"

using namespace engine;

auto reflection::register_material_component() -> void {
    auto factory = entt::meta<MaterialComponent>();
    factory.prop("name"_hs, "MaterialComponent");

    factory.data<&MaterialComponent::shininess>("shininess"_hs).prop("name"_hs, "shininess");
    factory.data<&MaterialComponent::reflection>("reflection"_hs).prop("name"_hs, "reflection");
    factory.data<&MaterialComponent::refraction>("refraction"_hs).prop("name"_hs, "refraction");
    factory.data<&MaterialComponent::mesh>("mesh"_hs).prop("name"_hs, "mesh");
    factory.data<&MaterialComponent::diffuse>("diffuse"_hs).prop("name"_hs, "diffuse");
    factory.data<&MaterialComponent::specular>("specular"_hs).prop("name"_hs, "specular");
    factory.data<&MaterialComponent::normal>("normal"_hs).prop("name"_hs, "normal");
}