#pragma once

namespace engine {

/**
 * Any derived class will be copy forbidden.
 *
 * It is easier than writing everything by hand and makes the intent explicit and clear.
 */
class noncopyable {
  public:
    noncopyable(const noncopyable&) noexcept = delete;
    auto operator=(const noncopyable&) -> noncopyable& = delete;

    noncopyable() = default;
    ~noncopyable() noexcept = default;

    noncopyable(noncopyable&&) noexcept = default;
    auto operator=(noncopyable&&) noexcept -> noncopyable& = default;
};

}  // namespace engine