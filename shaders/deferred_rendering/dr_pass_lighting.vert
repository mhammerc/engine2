#version 410 core

layout(location = 0) in vec2 vertex_position;
layout(location = 2) in vec2 vertex_uv;

out vec2 fragment_uv;

void main() {
    gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);
    fragment_uv = vertex_uv;
}