#pragma once

#include "../opengl/opengl.h"

struct Cube {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale {1.F};
};