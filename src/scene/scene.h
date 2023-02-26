#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../graphics/shader_program.h"
#include "../graphics/skybox.h"
#include "../to_be_deleted/vertex_array_object.h"
#include "camera.h"
#include "game_object.h"
#include "light.h"

#define POST_PROCESS_INVERSE (1 << 0)
#define POST_PROCESS_GRAYSCALE (1 << 1)
#define POST_PROCESS_SEPIA (1 << 2)
#define POST_PROCESS_BLUR (1 << 3)
#define POST_PROCESS_SHARPEN (1 << 4)
#define POST_PROCESS_EDGE_DETECTION (1 << 5)

namespace engine {

/**
 * A `Scene` represent the "world". Everything that currently lives into
 * the world such as objects, lights, behaviors and more.
 */
class Scene {
  public:
    Scene();
    auto draw(GLFWwindow* window, float delta_time, Skybox* skybox) -> void;
    auto draw_nodes(GameObject* game_object) -> void;
    auto draw_nodes_outline() -> void;
    auto draw_nodes_normals() -> void;
    engine::GameObject world;

    std::array<Light, 10> lights {};

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

}  // namespace engine