#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "../opengl/opengl.h"
#include "image.h"

class Texture {
  public:
    enum Type { Diffuse, Specular, Color, DepthStencil };

    static auto from_file(const std::filesystem::path& path, Type type, bool flip = true) -> std::optional<Texture>;
    static auto from_empty(Type type, int width, int height, int multisample) -> std::unique_ptr<Texture>;

    auto activate_as(int index) -> void;

    ~Texture() noexcept;

    Texture(const Texture&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;

    Texture(Texture&&) noexcept;
    auto operator=(Texture&&) noexcept -> Texture&;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto type() const -> Type;

  private:
    explicit Texture(GLuint handle, Type type);

    GLuint _handle = 0;
    Type _type;
};
