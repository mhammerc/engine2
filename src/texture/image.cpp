#include "image.h"

#include "spdlog/spdlog.h"

#include <stb_image/stb_image.h>

auto Image::from_file(const std::filesystem::path &path, std::optional<Channels> desired_channels, bool flip) -> std::optional<
	Image> {
  int width = 0;
  int height = 0;
  int number_of_channels = 0;

  int desired_channels_numeric = 0;
  if (desired_channels.has_value()) {
	desired_channels_numeric = *desired_channels;
  }

  stbi_set_flip_vertically_on_load(flip);
  unsigned char *data =
	  stbi_load(path.c_str(), &width, &height, &number_of_channels, desired_channels_numeric);

  if (!data) {
	auto error = stbi_failure_reason();
	spdlog::error("Could not load texture '{}' ({}).", path.c_str(), error);

	return std::nullopt;
  }

  auto image = Image(width, height,
					 static_cast<Channels>(*desired_channels || number_of_channels), data);

  return image;
}

Image::Image(int width, int height, Channels channels, unsigned char const *const data)
	: width(width), height(height), channels(channels), data(data) {}

Image::Image(Image &&from) noexcept
	: width(from.width), height(from.height), channels(from.channels), data(from.data) {
  from.data = nullptr;
}

Image::~Image() noexcept {
  if (data) {
	stbi_image_free((void *)data);
	data = nullptr;
  }
}