#pragma once

#include <entt/entt.hpp>

#include "../../common.h"

namespace engine::ui::internal {
auto on_property(entt::meta_any& instance, entt::meta_data const& member) -> void;

/**
 * Read a 'prop' from any meta type.
 */
template<typename T, typename meta>
auto prop(std::string_view name, meta const& type) -> T {
    if (auto prop = type.prop(entt::hashed_string {name.data()}); prop) {
        auto value = prop.value().template cast<T>();

        if (value) {
            return value;
        }
    }

    return nullptr;
}

auto on_property_quaternion(const char* name, glm::quat* value) -> bool;

}  // namespace engine::ui::internal