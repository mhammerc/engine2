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
  glBindTexture(GL_TEXTURE_2D, _handle);
}

Texture::~Texture() noexcept {
  if (_handle != 0U) {
	glDeleteTextures(1, &_handle);
  }
}

Texture::Texture(Texture &&from) noexcept : _handle(from._handle), _type(from._type) {
  from._handle = 0;
}

auto Texture::operator=(Texture &&from) noexcept -> Texture & {
  _handle = from._handle;
  _type = from._type;

  from._handle = 0;

  return *this;
}

Texture::Texture(GLuint handle, Type type) : _handle(handle), _type(type) {}

auto Texture::handle() const -> GLuint {
  return _handle;
}

auto Texture::type() const -> Type {
  return _type;
}