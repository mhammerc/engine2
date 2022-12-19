#pragma once

#include <filesystem>
#include <optional>

class Image {
 public:
  enum Channels {
	RGB = 3,
	RGBA = 4,
	G = 1, // Gray
	GA = 2, // Gray Alpha
  };

  const int width;
  const int height;
  const Channels channels;

  const unsigned char *data;

  static auto from_file(const std::filesystem::path &path, std::optional<Channels> desired_channels) ->
  std::optional<Image>;

  ~Image() noexcept;
  Image(const Image &) = delete;
  Image(Image &&) noexcept;

 private:
  Image(int width, int height, Channels channels, const unsigned char *data);
};