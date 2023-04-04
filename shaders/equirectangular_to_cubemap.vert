#version 410 core

layout(location = 0) in vec3 aPos;

out vec3 local_pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    local_pos = aPos;
    gl_Position = projection * view * vec4(local_pos, 1.0);
}