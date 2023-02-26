#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../graphics/opengl/shader_program.h"
#include "../graphics/opengl/skybox.h"
#include "../graphics/opengl/vertex_array_object.h"
#include "camera.h"
#include "game_object.h"
#include "light.h"
#include "node.h"

#define POST_PROCESS_INVERSE (1 << 0)
#define POST_PROCESS_GRAYSCALE (1 << 1)
#define POST_PROCESS_SEPIA (1 << 2)
#define POST_PROCESS_BLUR (1 << 3)
#define POST_PROCESS_SHARPEN (1 << 4)
#define POST_PROCESS_EDGE_DETECTION (1 << 5)

/**
 * A `Scene` represent the "world". Everything that currently lives into
 * the world such as objects, lights, behaviors and more.
 */
class Scene {
  public:
    Scene();
    auto draw(GLFWwindow* window, float delta_time, glm::mat4 projection, Skybox* skybox) -> void;
    auto draw_nodes(glm::mat4 projection) -> void;
    auto draw_nodes_outline(glm::mat4 projection) -> void;
    auto draw_nodes_normals(glm::mat4 projection) -> void;
    GameObject world;

    std::array<Light, 10> lights {};
    std::vector<Node> nodes;

    std::shared_ptr<Camera> camera;

    bool flashlight = true;
    bool wireframe = false;
    bool mirror = false;
    bool glass = false;
    bool outline = false;
    bool inverse = false;
    bool black_and_white = false;
    bool sepia = false;
    bool blur = false;
    bool sharpen = false;
    bool edge_dectection = false;
    bool show_normals = false;
    float explosion = 0.F;

    std::shared_ptr<ShaderProgram> outline_shader;
    std::shared_ptr<ShaderProgram> normal_shader;
};