#pragma once

#include <filesystem>
#include <optional>

#include "../../common.h"

namespace engine {

class TextureImage {
  public:
    enum Channels {
        RGB = 3,
        RGBA = 4,
        G = 1,  // Gray
        GA = 2,  // Gray Alpha
    };

  public:
    static auto from_file(const std::filesystem::path& path, std::optional<Channels> desired_channels, bool flip = true)
        -> std::unique_ptr<TextureImage>;

    ~TextureImage() noexcept;

    TextureImage(const TextureImage&) = delete;
    auto operator=(const TextureImage&) -> TextureImage& = delete;

    TextureImage(TextureImage&&) noexcept;
    auto operator=(TextureImage&&) noexcept -> TextureImage&;

  public:
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto channels() const -> Channels;
    [[nodiscard]] auto data() const -> void const*;
    [[nodiscard]] auto is_hdr() const -> bool;

  private:
    TextureImage(vec2i size, Channels channels, bool is_hdr, void* data);

    auto release() -> void;

  private:
    vec2i _size;
    Channels _channels;
    bool _is_hdr;

    /**
     * May be `u8 const *` or `float const *`
     */
    void* _data;
};

}  // namespace engine