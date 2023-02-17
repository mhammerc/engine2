#pragma once

#include <memory>

#include "../model/model.h"
#include "../opengl/opengl.h"

/**
 * `Node` is a model with world properties: position, rotation, scale, ...
 */
class Node {
  public:
    auto draw() -> void;

    auto model() -> std::shared_ptr<Model>&;
    auto shader() -> std::shared_ptr<ShaderProgram>&;

    auto position() -> glm::vec3&;
    auto rotation() -> glm::vec3&;
    auto scale() -> glm::vec3&;

  private:
    std::shared_ptr<Model> _model;
    std::shared_ptr<ShaderProgram> _shader;

    glm::vec3 _position {};
    glm::vec3 _rotation {};
    glm::vec3 _scale = glm::vec3(1.F);

    auto setShaderModel() -> void;
};