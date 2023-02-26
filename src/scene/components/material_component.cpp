#include "material_component.h"

auto MaterialComponent::shininess() -> float& {
    return _shininess;
}

auto MaterialComponent::reflection() -> float& {
    return _reflection;
}

auto MaterialComponent::refraction() -> float& {
    return _refraction;
}

auto MaterialComponent::textures() -> std::map<i32, Texture*>& {
    return _textures;
}