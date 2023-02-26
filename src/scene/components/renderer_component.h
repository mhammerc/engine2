#pragma once

#include <map>

#include "../../common.h"
#include "../../graphics/texture.h"
#include "../component.h"

namespace engine {

class RendererComponent: public Component {
  public:
    auto on_draw() -> void override;
};

}  // namespace engine