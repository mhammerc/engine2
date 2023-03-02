#pragma once

#include <string>

namespace engine {

struct NameComponent {
    std::string name;

    bool enabled = true;
};

namespace reflection {
    auto register_name_component() -> void;
}

};  // namespace engine