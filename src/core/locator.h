#pragma once

#include <cassert>
#include <memory>
#include <utility>

namespace engine {

/**
 * This class acts like a global Context.
 *
 * Ensure your services are copy forbidden to avoid mistakes.
 *
 * This class was heavily-inspired by EnTT (https://github.com/skypjack/entt).
 */
template<typename Service>
class locator {
  public:
    using type = Service;

    locator() = delete;
    ~locator() = delete;
    locator(const locator&) = delete;
    auto operator=(const locator&) -> locator& = delete;
    locator(locator&&) = delete;
    auto operator=(locator&&) -> locator& = delete;

    [[nodiscard]] static auto has_value() noexcept -> bool {
        return _pointer != nullptr;
    }

    [[nodiscard]] static auto value() noexcept -> type& {
        assert(has_value());

        return *_pointer;
    }

    template<typename Impl = Service, typename... Args>
    static auto emplace(Args&&... args) -> type& {
        _pointer = std::make_unique<Impl>(std::forward<Args>(args)...);

        return *_pointer;
    }

  private:
    static inline std::unique_ptr<type> _pointer = nullptr;
};

}  // namespace engine