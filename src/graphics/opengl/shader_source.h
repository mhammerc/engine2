#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "../../common.h"

struct ShaderSource {
    enum Type { UNKNOWN, FRAGMENT, VERTEX, GEOMETRY };

    static auto from_file(Type type, const std::filesystem::path& path) -> std::unique_ptr<ShaderSource>;

    ~ShaderSource() noexcept;

    ShaderSource(const ShaderSource&) = delete;
    auto operator=(const ShaderSource&) -> ShaderSource& = delete;

    ShaderSource(ShaderSource&&) noexcept;
    auto operator=(ShaderSource&&) noexcept -> ShaderSource&;

  public:
    /**
     * @return true on success, false otherwise.
     */
    auto compile() -> bool;

    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto source() const -> std::string;
    [[nodiscard]] auto path() const -> std::filesystem::path;
    [[nodiscard]] auto handle() const -> u32;

  private:
    ShaderSource() = default;

  private:
    Type _type = Type::UNKNOWN;
    std::string _source;
    std::filesystem::path _path;

    u32 _handle = 0;

  private:
    auto release() -> void;
};