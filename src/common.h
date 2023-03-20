#pragma once

// NOLINTBEGIN(unused-includes)

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <cinttypes>
#include <cstddef>
#include <entt/core/hashed_string.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>

#include "conf.h"

namespace engine {

#define ENGINE_CODE_ERROR(message) SPDLOG_ERROR(message);

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using byte = std::byte;  // use it to represent "buffer-like" data instead of unsigned char
using size = std::size_t;
using index = std::ptrdiff_t;  // use it instead of size_t when you are indexing arrays and alike

// so it's easier to use vec without prefixing with 'glm::'
using vec2i = glm::vec<2, int>;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;
using mat4 = glm::mat4;

using namespace entt::literals;  // so we can use '_hs' suffix on C strings for EnTT calls

}  // namespace engine

// NOLINTEND(unused-includes)