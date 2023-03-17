#include "texture.h"

#include "texture_image.h"

using namespace engine;

auto Texture::from_file(const std::filesystem::path& path, Type type, bool flip) -> std::unique_ptr<Texture> {
    auto image = TextureImage::from_file(path, TextureImage::Channels::RGB, flip);

    if (!image) {
        return nullptr;
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint internalformat = GL_RGB;

    if (type == Type::Diffuse) {
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

    return std::make_unique<Texture>(Texture(texture, image->size(), type, path.string()));
}

auto Texture::from_empty(const std::string& name, Type type, vec2i size, int multisample) -> std::unique_ptr<Texture> {
    GLuint handle = 0;

    int const target = multisample == 0 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

    glGenTextures(1, &handle);
    glBindTexture(target, handle);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint internalformat = GL_RGB;
    GLenum format = GL_RGB;
    GLenum textureType = GL_UNSIGNED_BYTE;

    if (type == Type::DepthStencil) {
        internalformat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        textureType = GL_UNSIGNED_INT_24_8;
    }

    if (multisample == 0) {
        glTexImage2D(target, 0, internalformat, size.x, size.y, 0, format, textureType, nullptr);
    } else {
        glTexImage2DMultisample(target, multisample, internalformat, size.x, size.y, GL_TRUE);
    }

    glBindTexture(target, 0);

    return std::make_unique<Texture>(Texture(handle, size, type, name));
}

auto Texture::activate_as(u32 index, bool disable) -> void {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, disable ? 0 : _handle);
}

Texture::~Texture() noexcept {
    release();
}

auto Texture::release() -> void {
    if (_handle != 0) {
        glDeleteTextures(1, &_handle);
        _handle = 0;
    }
}

Texture::Texture(Texture&& from) noexcept :
    _handle(from._handle),
    _size(from._size),
    _type(from._type),
    _name(std::move(from._name)) {
    from._handle = 0;
    from._size = {0, 0};
}

auto Texture::operator=(Texture&& from) noexcept -> Texture& {
    release();

    _handle = from._handle;
    _size = from._size;
    _type = from._type;
    _name = std::move(from._name);

    from._handle = 0;
    from._size = {0, 0};

    return *this;
}

Texture::Texture(GLuint handle, vec2i size, Type type, std::string name) :
    _handle(handle),
    _size(size),
    _type(type),
    _name(std::move(name)) {}

auto Texture::handle() const -> GLuint {
    return _handle;
}

auto Texture::size() const -> vec2i {
    return _size;
}

auto Texture::type() const -> Type {
    return _type;
}

auto Texture::name() -> std::string& {
    return _name;
}