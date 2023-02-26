#pragma once

#include <filesystem>
#include <optional>

#include "../../common.h"

class Image {
  public:
    enum Channels {
        RGB = 3,
        RGBA = 4,
        G = 1,  // Gray
        GA = 2,  // Gray Alpha
    };

  public:
    static auto from_file(const std::filesystem::path& path, std::optional<Channels> desired_channels, bool flip = true)
        -> std::unique_ptr<Image>;

    ~Image() noexcept;

    Image(const Image&) = delete;
    auto operator=(const Image&) -> Image& = delete;

    Image(Image&&) noexcept;
    auto operator=(Image&&) noexcept -> Image&;

  public:
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto channels() const -> Channels;
    [[nodiscard]] auto data() const -> u8 const*;

  private:
    Image(vec2i size, Channels channels, u8 const* data);

    auto release() -> void;

  private:
    vec2i _size;
    Channels _channels;

    u8 const* _data;
};