#pragma once

#include "../common.h"

/**
 * Conversion functions:
 *  - ImVec2 <-> engine::vec2
 *  - ImVec4 <-> engine::vec4
 */

#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(const engine::vec2& f) : x(f.x), y(f.y) {} \
    operator engine::vec2() const { return engine::vec2(x, y); }

#define IM_VEC4_CLASS_EXTRA \
    constexpr ImVec4(const engine::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
    operator engine::vec4() const { return engine::vec4(x, y, z, w); }

#include <imgui/imgui.h>