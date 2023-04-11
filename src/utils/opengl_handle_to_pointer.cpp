#include "opengl_handle_to_pointer.h"

using namespace engine;

auto ::engine::opengl_handle_to_pointer(u32 handle) -> void* {
    return reinterpret_cast<void*>(static_cast<u64>(handle));
}
