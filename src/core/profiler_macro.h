#include "profiler.h"

namespace engine {

// Define them as different symbols so entt::locator can locate two seperate instances.
// `using ProfilerFrame = Profiler;` doesn't work in this case.
class ProfilerFrame: public Profiler {};
class ProfilerPermanent: public Profiler {};

}  // namespace engine

/**
 * Use this macro whenever you want to create and measure a new block.
 * It is considered the impact on performance is null. It profiles only when asked to.
 *
 * Ex:
 * PROFILE_BLOCK("DeferredRenderer::draw()");
 */
#define PROFILER_BLOCK(...) \
    [[maybe_unused]] auto&& __profiler_scope = \
        ::entt::locator<::engine::ProfilerFrame>::value().block(::fmt::format(__VA_ARGS__));

/**
 * This is the same as PROFILER_BLOCK except it will be permanent and not reset for each frame.
 * You can use this macro to profile resources loading and unique tasks that are never repeated again.
 */
#define PROFILER_PERMANENT_BLOCK(...) \
    [[maybe_unused]] auto&& __permanent_profiler_scope = \
        ::entt::locator<::engine::ProfilerPermanent>::value().block(::fmt::format(__VA_ARGS__));
