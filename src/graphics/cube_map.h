#pragma once

#include <array>
#include <filesystem>

#include "../common.h"

namespace engine {

class CubeMap {
  public:
    static auto from_files(const std::array<std::filesystem::path, 6>& files) -> std::unique_ptr<CubeMap>;

    auto activate_as(u32 index, bool disable = false) -> void;

    ~CubeMap() noexcept;

    CubeMap(const CubeMap&) = delete;
    auto operator=(const CubeMap&) -> CubeMap& = delete;

    CubeMap(CubeMap&&) noexcept;
    auto operator=(CubeMap&&) noexcept -> CubeMap&;

    [[nodiscard]] auto handle() const -> u32;

  private:
    explicit CubeMap(u32 handle);

    auto release() -> void;

  private:
    u32 _handle = 0;
};

}  // namespace engine