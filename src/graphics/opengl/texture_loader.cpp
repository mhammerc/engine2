#include "texture_loader.h"

using namespace engine;

auto texture_loader::operator()(
    std::string const& /*name*/,
    const std::filesystem::path& path,
    Texture::Type type,
    bool flip
) const -> texture_loader::result_type {
    auto texture = Texture::from_file(path, type, flip);

    return std::move(texture);
}

auto texture_loader::operator()(std::string const& /*name*/, Texture::Type type, vec2i size, int multisample) const
    -> texture_loader::result_type {
    auto texture = Texture::from_empty(type, size, multisample);

    return std::move(texture);
}