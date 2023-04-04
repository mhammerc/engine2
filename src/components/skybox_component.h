#include <memory>

#include "../graphics/texture/texture.h"

namespace engine {
struct SkyboxComponent {
    std::shared_ptr<Texture> cubemap;
};

namespace reflection {
    auto register_skybox_component() -> void;
}
}  // namespace engine