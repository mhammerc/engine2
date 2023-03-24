#include "texture_internal.h"

using namespace engine;
using namespace engine::internal;

auto texture::OpenGLTextureProperties::from_texture_format(Texture::Format format) -> OpenGLTextureProperties {
    auto properties = OpenGLTextureProperties();

    if (format == Texture::Format::RGB) {
        properties.internalformat = GL_RGB;
        properties.format = GL_RGB;
        properties.type = GL_UNSIGNED_BYTE;
    } else if (format == Texture::Format::SRGB) {
        properties.internalformat = GL_SRGB;
        properties.format = GL_RGB;
        properties.type = GL_UNSIGNED_BYTE;
    } else if (format == Texture::Format::RGBA16F) {
        properties.internalformat = GL_RGBA16F;
        properties.format = GL_RGBA;
        properties.type = GL_FLOAT;
    } else if (format == Texture::Format::RGBA32F) {
        properties.internalformat = GL_RGBA32F;
        properties.format = GL_RGBA;
        properties.type = GL_FLOAT;
    } else if (format == Texture::Format::DepthStencil) {
        properties.internalformat = GL_DEPTH24_STENCIL8;
        properties.format = GL_DEPTH_STENCIL;
        properties.type = GL_UNSIGNED_INT_24_8;
    } else if (format == Texture::Format::Depth) {
        properties.internalformat = GL_DEPTH_COMPONENT;
        properties.format = GL_DEPTH_COMPONENT;
        properties.type = GL_FLOAT;
    } else {
        ENGINE_CODE_ERROR("missing case");
    }

    return properties;
}

auto texture::opengl_target_from_type(Texture::Type type) -> GLenum {
    if (type == Texture::Type::Texture2D) {
        return GL_TEXTURE_2D;
    }
    if (type == Texture::Type::CubeMap) {
        return GL_TEXTURE_CUBE_MAP;
    }

    ENGINE_CODE_ERROR("missing case");

    return GL_TEXTURE_2D;
}

auto texture::tex_image_2d(GLenum target, OpenGLTextureProperties properties, vec2i size, const void* data) -> void {
    glTexImage2D(target, 0, properties.internalformat, size.x, size.y, 0, properties.format, properties.type, data);
}