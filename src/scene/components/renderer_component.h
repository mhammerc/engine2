#pragma once

#include <map>

#include "../../common.h"
#include "../../graphics/opengl/texture.h"
#include "../component.h"

class RendererComponent: public Component {
  protected:
    auto on_draw() -> void override;
};
