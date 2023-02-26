#include "game_object.h"

namespace engine {

GameObject::GameObject(std::string name) : _name(std::move(name)) {}

auto GameObject::name() -> std::string& {
    return _name;
}

auto GameObject::unique_index() const -> size_t {
    return _unique_index;
}

auto GameObject::components() -> std::unordered_map<std::type_index, std::unique_ptr<Component>> const& {
    return _components;
}

auto GameObject::childrens() -> std::vector<std::unique_ptr<GameObject>>& {
    return _childrens;
}

auto GameObject::get_next_global_index() -> size_t {
    static size_t global_index = -1;
    return ++global_index;
}

}  // namespace engine