#include "image.h"

#include "spdlog/spdlog.h"
#include "stb_image/stb_image.h"

auto Image::from_file(const std::filesystem::path& path, std::optional<Channels> desired_channels, bool flip)
    -> std::unique_ptr<Image> {
    vec2i size(0);
    int number_of_channels = 0;

    int desired_channels_numeric = 0;
    if (desired_channels.has_value()) {
        desired_channels_numeric = *desired_channels;
    }

    stbi_set_flip_vertically_on_load(flip ? 1 : 0);
    unsigned char* data = stbi_load(path.c_str(), &size.x, &size.y, &number_of_channels, desired_channels_numeric);

    if (!data) {
        const auto* error = stbi_failure_reason();
        spdlog::error("Could not load texture '{}' ({}).", path.c_str(), error);

        return nullptr;
    }

    auto image =
        Image(size, static_cast<Channels>(desired_channels.has_value() ? *desired_channels : number_of_channels), data);

    return std::make_unique<Image>(std::move(image));
}

Image::Image(vec2i size, Channels channels, u8 const* data) : _size(size), _channels(channels), _data(data) {}

Image::Image(Image&& from) noexcept : _size(from._size), _channels(from._channels), _data(from._data) {
    from._data = nullptr;
}

auto Image::operator=(Image&& from) noexcept -> Image& {
    release();

    _size = from._size;
    _channels = from._channels;
    _data = from._data;

    from._data = nullptr;

    return *this;
}

Image::~Image() noexcept {
    release();
}

auto Image::release() -> void {
    if (_data != nullptr) {
        stbi_image_free((void*)_data);
        _data = nullptr;
    }
}

auto Image::size() const -> vec2i {
    return _size;
}

auto Image::channels() const -> Channels {
    return _channels;
}

auto Image::data() const -> u8 const* {
    return _data;
}