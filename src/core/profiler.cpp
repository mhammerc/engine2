#include "profiler.h"

#include "../conf.h"

using namespace engine;

Profiler::CallbackOnDestruction::~CallbackOnDestruction() noexcept {
    if (_callback) {
        _callback();
    }
}

Profiler::CallbackOnDestruction::CallbackOnDestruction(CallbackOnDestruction&& from) noexcept :
    _callback(std::move(from._callback)) {
    from._callback = nullptr;
}

auto Profiler::CallbackOnDestruction::operator=(CallbackOnDestruction&& from) noexcept -> CallbackOnDestruction& {
    _callback = std::move(from._callback);

    from._callback = nullptr;

    return *this;
}

auto Profiler::root_block() -> Block* {
    return &_root_block;
}

auto Profiler::enable_for_next_frame() -> void {
    _is_enabled_for_next_frame = true;
}

auto Profiler::start_frame() -> void {
    if (!_is_enabled_for_next_frame || _is_enabled) {
        return;
    }

    _is_enabled = true;
    _is_enabled_for_next_frame = false;

    // Delete the previous root block (and all its hierarchy).
    // Create a new root block.
    _root_block = Block();
    _root_block.start_time = _now();

    // Point the latest block as being the root block
    _current_block = &_root_block;
}

auto Profiler::end_frame() -> void {
    if (!_is_enabled) {
        return;
    }

    _is_enabled = false;
    _current_block = nullptr;

    _root_block.end_time = _now();
}

auto Profiler::block(std::string const& name) -> CallbackOnDestruction {
    if constexpr (!ENGINE_DEBUG) {
        // TODO: because this is constexpr we could optimize better by using a function specialization
        // or something that will entirely disable everything.
        return CallbackOnDestruction {};
    }

    if (!_current_block || !_is_enabled) {
        return CallbackOnDestruction {};
    }

    Block new_block;
    new_block.name = name;
    new_block.start_time = _now();
    new_block.parent = _current_block;

    auto& new_block_addr = _current_block->childrens.emplace_back(std::move(new_block));

    _current_block = &new_block_addr;

    auto callback_on_destruction = CallbackOnDestruction {};

    callback_on_destruction._callback = [&]() {
        if (!_current_block) {
            return;
        }

        _current_block->end_time = _now();
        _current_block = _current_block->parent;
    };

    return callback_on_destruction;
}

auto Profiler::_now() -> time_point {
    return std::chrono::steady_clock::now();
}