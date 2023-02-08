#include "texture.h"

auto Texture::from_file(const std::filesystem::path &path, Type type) -> std::optional<Texture> {
  auto image = Image::from_file(path, Image::Channels::RGB);

  if (!image.has_value()) {
	return std::nullopt;
  }

  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture(texture, type);
}

auto Texture::activate_as(int index) -> void {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::~Texture() noexcept {
  if (handle != 0U) {
	glDeleteTextures(1, &handle);
  }
}

Texture::Texture(Texture &&from) noexcept : handle(0) {
  handle = from.handle;
  type = from.type;

  from.handle = 0;
}

auto Texture::operator=(Texture &&from) noexcept -> Texture & {
  handle = from.handle;
  type = from.type;

  from.handle = 0;

  return *this;
}

Texture::Texture(GLuint handle, Type type) : handle(handle), type(type) {}
