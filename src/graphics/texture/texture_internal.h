#pragma once

#include "../../common.h"
#include "texture.h"

/**
 * Internal functions and more that have no home in public classes.
 */
namespace engine::internal::texture {

/**
 * From a format (eg. RGB or SRGB), get the various properties for an OpenGL texture (eg. GL_RGB, GL_UNSIGNED_BYTE).
 */
struct OpenGLTextureProperties {
    static auto from_texture_format(Texture::Format format) -> OpenGLTextureProperties;

    GLint internalformat = 0;
    GLenum format = 0;
    GLenum type = 0;
};

/**
 * A plain wrapper around glTexImage2D easier to use.
 */
auto tex_image_2d(GLenum target, OpenGLTextureProperties properties, vec2i size, const void* data) -> void;

/**
 * Convert a Texture::Type (eg. Type::Texture2D) to an OpenGL texture target (eg. GL_TEXTURE_2D).
 */
auto opengl_target_from_type(Texture::Type type) -> GLenum;

}  // namespace engine::internal::texture