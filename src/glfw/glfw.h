#pragma once

#include "../opengl/opengl.h"
#include "../camera/camera.h"
#include <GLFW/glfw3.h>

GLFWwindow *init_glfw_and_opengl();

void processInputs(float delta_time, GLFWwindow *window, Camera &camera);

void terminate_glfw(GLFWwindow *window);
