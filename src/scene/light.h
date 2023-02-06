#pragma once

#include "../opengl/opengl.h"

struct Light {
  enum Type {
	Unset = 0x0000,
	Directional = 0x0001,
	Point = 0x0002,
	Spot = 0x0003,
  };

  Type type = Type::Unset;

  glm::vec3 position; // spot, point
  glm::vec3 direction;// spot, directional

  float innerCutOff;// spot
  float outerCutOff;// spot

  float constant = 1.F;// spot, point
  float linear;        // spot, point
  float quadratic;     // spot, point

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};