#pragma once

#include <array>
#include <filesystem>
#include <string>

#include "../common.h"

namespace engine {

class CubeMap {
  public:
    static auto from_files(std::string const& name, const std::array<std::filesystem::path, 6>& files)
        -> std::unique_ptr<CubeMap>;

    auto activate_as(u32 index, bool disable = false) -> void;

    ~CubeMap() noexcept;

    CubeMap(const CubeMap&) = delete;
    auto operator=(const CubeMap&) -> CubeMap& = delete;

    CubeMap(CubeMap&&) noexcept;
    auto operator=(CubeMap&&) noexcept -> CubeMap&;

    [[nodiscard]] auto handle() const -> u32;
    [[nodiscard]] auto name() -> std::string&;

  private:
    explicit CubeMap(u32 handle, std::string name);

    auto release() -> void;

  private:
    u32 _handle = 0;
    std::string _name;
};

}  // namespace engine