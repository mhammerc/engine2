#pragma once

#include <spdlog/fmt/fmt.h>

#include <chrono>
#include <entt/locator/locator.hpp>
#include <functional>
#include <string>
#include <vector>

#include "noncopyable.h"

namespace engine {

/**
 * This is a very simple profiler with the following features:
 *
 *  - Does nothing unless enabled for the current "frame"
 *  - Tree-like hierarchy
 *  - Simple to use and read
 *  - Measure the execution time of each declared "blocks".
 * 
 * This profile have a very limited feature set but fit really well into what we need.
 */
class Profiler: public noncopyable {
  public:
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;

    /**
     * A simple class that call a callback on destruction.
     * Useful for measuring the time when it get destructed.
     */
    struct CallbackOnDestruction: public noncopyable {
        CallbackOnDestruction() = default;
        virtual ~CallbackOnDestruction() noexcept;

        CallbackOnDestruction(const CallbackOnDestruction&) noexcept = delete;
        auto operator=(const CallbackOnDestruction&) -> CallbackOnDestruction& = delete;

        CallbackOnDestruction(CallbackOnDestruction&&) noexcept;
        auto operator=(CallbackOnDestruction&& from) noexcept -> CallbackOnDestruction&;

      private:
        friend class Profiler;
        std::function<void()> _callback;
    };

    /**
     * Recursive structure to form a hierarchy of timed blocks.
     */
    struct Block {
        Block* parent = nullptr;
        std::vector<Block> childrens {};

        std::string name = "Frame";

        time_point start_time {};
        time_point end_time {};
    };

    /**
     * Unless you call this function, the profiler will not measure/save anything on the next start_frame().
     */
    auto enable_for_next_frame() -> void;

    /**
     * Start a new profiling frame.
     */
    auto start_frame() -> void;

    /**
     * End the current profiling frame.
     */
    auto end_frame() -> void;

    /**
     * Declare and measure a new block.
     * The block life time is measured:
     *  - from creation when block() is called
     *  - until destruction at end of the creation scope.
     */
    [[nodiscard]] auto block(std::string const& name) -> CallbackOnDestruction;

    auto root_block() -> Block*;

  private:
    bool _is_enabled_for_next_frame = false;
    bool _is_enabled = false;

    Block _root_block;
    Block* _current_block = nullptr;

    static auto _now() -> time_point;
};

/**
 * Use this macro whenever you want to create and measure a new block.
 * Ex:
 * PROFILE_BLOCK("DeferredRenderer::draw()");
 */
#define PROFILER_BLOCK(...) \
    [[maybe_unused]] auto&& scope = entt::locator<Profiler>::value().block(fmt::format(__VA_ARGS__));

}  // namespace engine