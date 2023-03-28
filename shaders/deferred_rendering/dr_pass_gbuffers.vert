#version 410 core

layout(location = 0) in vec3 _vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out VS_OUT {
    vec2 fragment_uv;
    vec3 fragment_normal_world;
    vec3 fragment_position_world;
}
vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 model_normal;

void main() {
    vec4 vertex_position = vec4(_vertex_position, 1.0);

    vs_out.fragment_position_world = (model * vertex_position).xyz;

    gl_Position = projection * view * model * vertex_position;

    vs_out.fragment_uv = vertex_uv;

    vs_out.fragment_normal_world = model_normal * vertex_normal;
}