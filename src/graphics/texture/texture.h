#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>

#include "../../common.h"

namespace engine {

/**
 * Represent an OpenGL Texture object.
 *
 * We currently support `TEXTURE_2D` and `CUBE_MAP`.
 */
class Texture {
  public:
    enum class Format { RGB, RGBA, SRGB, RGBA16F, RGBA32F, RGB16SNORM, Depth, DepthStencil };
    enum class Type { Texture2D, CubeMap };

  public:
    /**
     * Create a Texture2D from a single file.
     */
    static auto from_file_2d(std::filesystem::path const& path, Format format, bool flip = true)
        -> std::unique_ptr<Texture>;

    /**
     * Create a CubeMap from 6 files.
     */
    static auto from_files_cubemap(std::string const& name, std::array<std::filesystem::path, 6> const& files)
        -> std::unique_ptr<Texture>;

    /**
     * Create a CubeMap from a file which contain an equirectangular projection.
     *
     * The projection will be projected onto a complete CubeMap. Loading this way is expensive.
     */
    static auto from_file_cubemap(std::filesystem::path const& path, Format format, bool flip = true)
        -> std::unique_ptr<Texture>;

    /**
     * Create an empty texture of a given type.
     */
    static auto from_empty(std::string const& name, Type type, Format format, vec2i size) -> std::unique_ptr<Texture>;

  public:
    // Because this is an owned resource, we can not copy a texture.
    ~Texture() noexcept;
    Texture(const Texture&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;
    Texture(Texture&&) noexcept;
    auto operator=(Texture&&) noexcept -> Texture&;

  public:
    auto activate_as(u32 index, bool disable = false) -> void;

    [[nodiscard]] auto handle() const -> u32;
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto format() const -> Format;
    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto name() -> std::string&;

  private:
    Texture() = default;

    auto release() -> void;

  private:
    /**
     * OpenGL handle.
     */
    u32 _handle = 0;

    /**
     * @brief Size of mipmap 0 images of the texture.
     *
     * There is one mipmap 0 image for a Texture2D but there are 6 for a CubeMap.
     */
    vec2i _size {0, 0};

    Format _format = Format::RGB;

    Type _type = Type::Texture2D;

    std::string _name;
};

}  // namespace engine