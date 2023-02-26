#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "../../common.h"
#include "image.h"

class Texture {
  public:
    enum Type { Diffuse, Specular, Color, DepthStencil };

    static auto from_file(const std::filesystem::path& path, Type type, bool flip = true) -> std::unique_ptr<Texture>;
    static auto from_empty(Type type, vec2i size, int multisample) -> std::unique_ptr<Texture>;

    auto activate_as(u32 index) -> void;

    ~Texture() noexcept;

    Texture(const Texture&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;

    Texture(Texture&&) noexcept;
    auto operator=(Texture&&) noexcept -> Texture&;

    [[nodiscard]] auto handle() const -> u32;
    [[nodiscard]] auto type() const -> Type;

  private:
    explicit Texture(u32 handle, Type type);

    auto release() -> void;

  private:
    u32 _handle = 0;
    Type _type;
};
