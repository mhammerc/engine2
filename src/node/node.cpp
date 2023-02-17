#include "node.h"

auto Node::draw() -> void {
    if (_model == nullptr || _shader == nullptr) {
        return;
    }

    setShaderModel();

    _model->draw(*_shader);
}

auto Node::setShaderModel() -> void {
    if (_shader == nullptr) {
        return;
    }

    glm::mat4 transform = glm::mat4(1.0F);

    {
        transform = glm::translate(transform, _position);

        transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1.F, 0.F, 0.F));
        transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0.F, 1.F, 0.F));
        transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0.F, 0.F, 1.F));

        transform = glm::scale(transform, _scale);
    }

    const auto modelNormal = glm::mat3(glm::transpose(glm::inverse(transform)));

    _shader->setUniform("model", transform);
    _shader->setUniform("modelNormal", modelNormal);
}

auto Node::model() -> std::shared_ptr<Model>& {
    return _model;
}

auto Node::shader() -> std::shared_ptr<ShaderProgram>& {
    return _shader;
}

auto Node::position() -> glm::vec3& {
    return _position;
}

auto Node::rotation() -> glm::vec3& {
    return _rotation;
}

auto Node::scale() -> glm::vec3& {
    return _scale;
}