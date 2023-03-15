#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 FragmentPosition = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * vec4(FragmentPosition, 1.0);
}