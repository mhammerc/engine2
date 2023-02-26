#pragma once

#include <functional>

namespace engine {

/**
 * Run the given function in a loop and compute the delta_time.
 */
void game_loop(const std::function<void(float, bool&)>& loop);

}  // namespace engine