#include "texture.h"

#include "texture_image.h"

using namespace engine;

static constexpr std::array<int, 2> type_to_opengl {GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP};

auto Texture::from_file_2d(const std::filesystem::path& path, Format format, bool flip) -> std::unique_ptr<Texture> {
    auto image = TextureImage::from_file(path, TextureImage::Channels::RGB, flip);

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

    GLint internalformat = GL_RGB;

    if (format == Format::Color) {
        // TODO: currently, all color textures are GL_SRGB: albedo, specular, ...
        internalformat = GL_SRGB;
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalformat,
        image->size().x,
        image->size().y,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image->data()
    );
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

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB,
            image->size().x,
            image->size().y,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image->data()
        );
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
    texture->_format = Format::Color;
    texture->_type = Type::CubeMap;
    texture->_name = name;

    return texture;
}

auto Texture::from_empty(std::string const& name, Type type, Format format, vec2i size) -> std::unique_ptr<Texture> {
    int const target = type_to_opengl.at(type);

    u32 handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(target, handle);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint internalformat = 0;
    GLenum gl_format = 0;
    GLenum gl_type = 0;

    if (format == Format::Color) {
        internalformat = GL_RGB;
        gl_format = GL_RGB;
        gl_type = GL_UNSIGNED_BYTE;
    } else if (format == Format::DepthStencil) {
        internalformat = GL_DEPTH24_STENCIL8;
        gl_format = GL_DEPTH_STENCIL;
        gl_type = GL_UNSIGNED_INT_24_8;
    } else {
        SPDLOG_ERROR("[source %s] [function %!] [line %#] Missing match in pattern matching");
        return nullptr;
    }

    if (type == Type::Texture2D) {
        glTexImage2D(target, 0, internalformat, size.x, size.y, 0, gl_format, gl_type, nullptr);
    } else if (type == Type::CubeMap) {
        // Create each sides of the cube map
        for (int i = 0; i < 6; ++i) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                internalformat,
                size.x,
                size.y,
                0,
                gl_format,
                gl_type,
                nullptr
            );
        }
    } else {
        SPDLOG_ERROR("[source %s] [function %!] [line %#] Missing match in pattern matching");
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
    int const target = type_to_opengl.at(_type);

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
