#include "skybox.h"

#include "image.h"
#include "spdlog/spdlog.h"

static const std::array<float, 108> skybox_vertices = {
    // positions
    -1.0F, 1.0F,  -1.0F, -1.0F, -1.0F, -1.0F, 1.0F,  -1.0F, -1.0F,
    1.0F,  -1.0F, -1.0F, 1.0F,  1.0F,  -1.0F, -1.0F, 1.0F,  -1.0F,

    -1.0F, -1.0F, 1.0F,  -1.0F, -1.0F, -1.0F, -1.0F, 1.0F,  -1.0F,
    -1.0F, 1.0F,  -1.0F, -1.0F, 1.0F,  1.0F,  -1.0F, -1.0F, 1.0F,

    1.0F,  -1.0F, -1.0F, 1.0F,  -1.0F, 1.0F,  1.0F,  1.0F,  1.0F,
    1.0F,  1.0F,  1.0F,  1.0F,  1.0F,  -1.0F, 1.0F,  -1.0F, -1.0F,

    -1.0F, -1.0F, 1.0F,  -1.0F, 1.0F,  1.0F,  1.0F,  1.0F,  1.0F,
    1.0F,  1.0F,  1.0F,  1.0F,  -1.0F, 1.0F,  -1.0F, -1.0F, 1.0F,

    -1.0F, 1.0F,  -1.0F, 1.0F,  1.0F,  -1.0F, 1.0F,  1.0F,  1.0F,
    1.0F,  1.0F,  1.0F,  -1.0F, 1.0F,  1.0F,  -1.0F, 1.0F,  -1.0F,

    -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, 1.0F,  1.0F,  -1.0F, -1.0F,
    1.0F,  -1.0F, -1.0F, -1.0F, -1.0F, 1.0F,  1.0F,  -1.0F, 1.0F,
};

auto Skybox::from_files(const std::array<std::filesystem::path, 6>& files) -> std::unique_ptr<Skybox> {
    // Create cube map texture
    GLuint texture = 0;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        for (size_t i = 0; i < files.size(); ++i) {
            const auto& file = files.at(i);
            auto image = Image::from_file(file, Image::Channels::RGB, false);

            if (!image) {
                spdlog::error("Could not load create skybox face");
                continue;
            }

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                image->size().x,
                image->size().y,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                image->data()
            );
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // Create shader
    auto shader = ShaderProgram::from_vertex_and_fragment("../shaders/skybox.vert", "../shaders/skybox.frag");
    if (shader == nullptr) {
        spdlog::critical("could not create shader program for skybox.");
    }

    // Create VAO and VBO
    GLuint VAO = 0;
    GLuint VBO = 0;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float), skybox_vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    auto skybox = std::unique_ptr<Skybox>(new Skybox());
    skybox->_texture = texture;
    skybox->_VAO = VAO;
    skybox->_VBO = VBO;
    skybox->_files = files;
    skybox->_shader = std::move(shader);

    return skybox;
}

auto Skybox::draw(glm::mat4 projection, glm::mat4 view) -> void {
    glDepthFunc(GL_LEQUAL);

    // Remove translation from the view matrix
    view = glm::mat4(glm::mat3(view));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    _shader->set_uniform("skybox", 0);
    _shader->set_uniform("projection", projection);
    _shader->set_uniform("view", view);

    _shader->bind();
    glBindVertexArray(_VAO);

    glDrawArrays(GL_TRIANGLES, 0, skybox_vertices.size() / 3);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    _shader->unbind();
}

auto Skybox::activate_cubemap_as(int index) -> void {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
}

Skybox::~Skybox() noexcept {
    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }

    if (_VAO != 0) {
        glDeleteVertexArrays(1, &_VAO);
        _VAO = 0;
    }

    if (_VBO != 0) {
        glDeleteBuffers(1, &_VBO);
        _VBO = 0;
    }
}

Skybox::Skybox(Skybox&& from) noexcept :
    _texture(from._texture),
    _VAO(from._VAO),
    _VBO(from._VBO),
    _files(std::move(from._files)) {
    from._texture = 0;
    from._VAO = 0;
    from._VBO = 0;
}

auto Skybox::operator=(Skybox&& from) noexcept -> Skybox& {
    _texture = from._texture;
    from._texture = 0;

    _VAO = from._VAO;
    from._VAO = 0;

    _VBO = from._VBO;
    from._VBO = 0;

    _files = from._files;

    return *this;
}