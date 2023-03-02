#include "entity.h"

using namespace engine;

auto engine::duplicate_entity(entt::registry& registry, entt::entity source) -> entt::entity {
    auto destination = registry.create();

    for (auto [id, storage] : registry.storage()) {
        if (storage.contains(source)) {
            storage.emplace(destination, storage.get(source));
        }
    }

    return destination;
}