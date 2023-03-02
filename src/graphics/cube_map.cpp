#include "cube_map.h"

#include <spdlog/spdlog.h>

#include "texture_image.h"

using namespace engine;

auto CubeMap::from_files(const std::array<std::filesystem::path, 6>& files) -> std::unique_ptr<CubeMap> {
    GLuint handle = 0;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    for (size i = 0; i < files.size(); ++i) {
        const auto& file = files.at(i);
        auto image = TextureImage::from_file(file, TextureImage::Channels::RGB, false);

        if (!image) {
            spdlog::error("Could not load image of a cubemap face");
            continue;
        }

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

    return std::make_unique<CubeMap>(CubeMap(handle));
}

auto CubeMap::activate_as(u32 index, bool disable) -> void {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, disable ? 0 : _handle);
}

auto CubeMap::release() -> void {
    if (_handle != 0) {
        glDeleteTextures(1, &_handle);
        _handle = 0;
    }
}

CubeMap::~CubeMap() noexcept {
    release();
}

CubeMap::CubeMap(CubeMap&& from) noexcept : _handle(from._handle) {
    from._handle = 0;
}

auto CubeMap::operator=(CubeMap&& from) noexcept -> CubeMap& {
    release();

    _handle = from._handle;
    from._handle = 0;

    return *this;
}

CubeMap::CubeMap(u32 handle) : _handle(handle) {}