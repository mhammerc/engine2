#pragma once

#include <typeindex>
#include <typeinfo>

namespace engine {

/**
 * This function will always return the same ID for the same template.
 */
template<typename T>
auto type_index() -> std::type_index {
    auto const& type_id = typeid(T);

    return std::type_index(type_id);
}

}  // namespace engine