#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../utils/type_index.h"
#include "component.h"

namespace engine {

class GameObject {
  public:
    GameObject() = default;
    GameObject(std::string name);

    auto name() -> std::string&;
    [[nodiscard]] auto unique_index() const -> size_t;
    auto components() -> std::unordered_map<std::type_index, std::unique_ptr<Component>> const&;
    auto childrens() -> std::vector<std::unique_ptr<GameObject>>&;

    template<typename T>
    auto insert_component(std::unique_ptr<T> component) -> T* {
        static_assert(std::is_base_of_v<Component, T>, "A component must be derived from Component");
        auto type_id = type_index<T>();

        assert(component.get() != nullptr);
        assert(!_components.contains(type_id));

        component->_game_object = this;

        auto insertion = _components.insert({type_id, std::move(component)});

        auto pointer = insertion.first->second.get();

        return dynamic_cast<T*>(pointer);
    }

    template<typename T, typename... Args>
    auto emplace_component(Args&&... args) -> T* {
        auto instance = std::make_unique<T>(std::forward<Args>(args)...);
        return insert_component(std::move(instance));
    }

    template<typename T>
    auto remove_component() -> void {
        static_assert(std::is_base_of_v<Component, T>, "A component must be derived from Component");

        auto type_id = type_index<T>();
        auto it = _components.find(type_id);

        assert(it != _components.end());

        _components.erase(it);
    }

    template<typename T>
    auto component() -> T* {
        static_assert(std::is_base_of_v<Component, T>, "A component must be derived from Component");

        auto type_id = type_index<T>();
        auto it = _components.find(type_id);

        if (it == _components.end()) {
            return nullptr;
        }

        auto pointer = it->second.get();

        return dynamic_cast<T*>(pointer);
    }

  private:
    std::string _name = "GameObject";

    static auto get_next_global_index() -> size_t;
    const size_t _unique_index = get_next_global_index();

    std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;

    std::vector<std::unique_ptr<GameObject>> _childrens;
};

}  // namespace engine