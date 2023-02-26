#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <unordered_map>

#include "../common.h"

namespace engine {
/**
 * This class was heavily-inspired by EnTT (https://github.com/skypjack/entt).
 */
template<typename Type, typename Loader>
class resource_cache {
  public:
    using type = std::shared_ptr<Type>;
    using loader = Loader;

    static_assert(
        std::is_same_v<type, typename loader::result_type>,
        "The loader doesn't return the same resource or type (shared_ptr)."
    );

    resource_cache() = default;
    ~resource_cache() = default;
    resource_cache(const resource_cache&) = delete;
    auto operator=(const resource_cache&) -> resource_cache& = delete;

    /**
     * May return nullptr.
     */
    [[nodiscard]] auto get(std::string const& name) -> type {
        auto it = _resources.find(name);

        if (it == _resources.end()) {
            return nullptr;
        }

        return it->second;
    }

    template<typename... Args>
    [[nodiscard]] auto get_or_load(std::string const& name, Args&&... args) -> type {
        auto it = _resources.find(name);

        if (it == _resources.end()) {
            return load(name, std::forward<Args>(args)...);
        }

        return it->second;
    }

    template<typename... Args>
    auto load(std::string const& name, Args&&... args) -> type {
        type new_resource = _loader(name, std::forward<Args>(args)...);
        auto insertion = _resources.insert_or_assign(name, new_resource);

        if constexpr (ENGINE_DEBUG) {
            if (!insertion.second) {
                // it was an assignment, not an insertion.
                // which means the resource was already loaded.
                spdlog::warn("resource_cache: loaded resource '{}' twice or more.", name);
            }
        }

        return new_resource;
    }

    auto clear() -> void {
        _resources.clear();
    }

    auto erase(std::string const& name) -> void {
        _resources.erase(name);
    }

  private:
    Loader _loader;
    std::unordered_map<std::string, type> _resources;
};
}  // namespace engine
