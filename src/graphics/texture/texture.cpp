#include "texture.h"

#include "texture_image.h"
#include "texture_internal.h"

using namespace engine;
using namespace engine::internal::texture;

auto Texture::from_file_2d(const std::filesystem::path& path, Format format, bool flip) -> std::unique_ptr<Texture> {
    if (format != Format::RGB && format != Format::SRGB && format != Format::RGBA16F) {
        ENGINE_CODE_ERROR("only Format::RGB, Format::SRGB and Format::RGBA16F are supported for now.");
    }

    auto channels = format == Format::RGBA16F ? TextureImage::Channels::RGBA : TextureImage::Channels::RGB;

    auto image = TextureImage::from_file(path, channels, flip);

    if (!image) {
        return nullptr;
    }

    u32 handle = 0;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto properties = OpenGLTextureProperties::from_texture_format(format);
    tex_image_2d(GL_TEXTURE_2D, properties, image->size(), image->data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    auto texture = std::make_unique<Texture>(Texture());
    texture->_handle = handle;
    texture->_size = image->size();
    texture->_format = format;
    texture->_type = Type::Texture2D;
    texture->_name = path.string();

    return texture;
}

auto Texture::from_files_cubemap(std::string const& name, std::array<std::filesystem::path, 6> const& files)
    -> std::unique_ptr<Texture> {
    auto format = Format::RGB;
    auto properties = OpenGLTextureProperties::from_texture_format(format);

    u32 handle = 0;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    vec2i last_size;

    for (::engine::size i = 0; i < files.size(); ++i) {
        const auto& file = files.at(i);
        auto image = TextureImage::from_file(file, TextureImage::Channels::RGB, false);

        if (!image) {
            spdlog::error("Could not load cubemap face image '{}'.", file.string().c_str());
            continue;
        }

        if (i != 0 && last_size != image->size()) {
            spdlog::error("Not all cube map face are the same size (detected with '{}').", file.string());
        }

        last_size = image->size();
        tex_image_2d(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, properties, image->size(), image->data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    auto texture = std::make_unique<Texture>(Texture());
    texture->_handle = handle;
    texture->_size = last_size;
    texture->_format = format;
    texture->_type = Type::CubeMap;
    texture->_name = name;

    return texture;
}

auto Texture::from_empty(std::string const& name, Type type, Format format, vec2i size) -> std::unique_ptr<Texture> {
    auto const target = opengl_target_from_type(type);

    u32 handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(target, handle);

    auto properties = OpenGLTextureProperties::from_texture_format(format);

    if (type == Type::Texture2D) {
        tex_image_2d(target, properties, size, nullptr);
    } else if (type == Type::CubeMap) {
        // Create each sides of the cube map
        for (int i = 0; i < 6; ++i) {
            tex_image_2d(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, properties, size, nullptr);
        }
    } else {
        ENGINE_CODE_ERROR("missing case");
        return nullptr;
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(target, 0);

    auto texture = std::make_unique<Texture>(Texture());
    texture->_handle = handle;
    texture->_size = size;
    texture->_format = format;
    texture->_type = type;
    texture->_name = name;

    return texture;
}

Texture::~Texture() noexcept {
    release();
}

Texture::Texture(Texture&& from) noexcept :
    _handle(from._handle),
    _size(from._size),
    _format(from._format),
    _type(from._type),
    _name(std::move(from._name)) {
    from._handle = 0;
    from._name = "Moved texture";
}

auto Texture::operator=(Texture&& from) noexcept -> Texture& {
    release();

    _handle = from._handle;
    _size = from._size;
    _format = from._format;
    _type = from._type;
    _name = from._name;

    from._handle = 0;
    from._name = "Moved texture";

    return *this;
}

auto Texture::activate_as(u32 index, bool disable) -> void {
    if (index > 31) {
        spdlog::error("Texture::active_as index is out of range");
    }

    auto const target = opengl_target_from_type(_type);

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target, disable ? 0 : _handle);
}

auto Texture::release() -> void {
    if (_handle != 0) {
        glDeleteTextures(1, &_handle);
        _handle = 0;
    }
}

auto Texture::handle() const -> u32 {
    return _handle;
}

auto Texture::size() const -> vec2i {
    return _size;
}

auto Texture::format() const -> Format {
    return _format;
}

auto Texture::type() const -> Type {
    return _type;
}

auto Texture::name() -> std::string& {
    return _name;
}

auto Texture::filter_nearest() -> void {
    auto const target = opengl_target_from_type(_type);

    glBindTexture(target, _handle);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(target, 0);
}