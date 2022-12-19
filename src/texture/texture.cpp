#include "texture.h"

std::optional<Texture> Texture::from_file(const std::filesystem::path &path) {
  auto image = Image::from_file(path, Image::Channels::RGB);

  if (!image.has_value()) return std::nullopt;

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture(texture);
}

void Texture::activate_as(int index) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::~Texture() noexcept {
  if (handle) {
	glDeleteTextures(1, &handle);
  }
}

Texture::Texture(Texture &&from) noexcept {
  handle = from.handle;

  from.handle = 0;
}

Texture::Texture(GLuint handle) : handle(handle) {}