#version 410 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;

void main() {
    // mvp
    // model view projection
    // model = local -> world
    // view = world -> camera view
    // projection = camera view -> perspective ?
    // TODO: understand mvp for shadows
    gl_Position = model * vec4(aPos, 1.0);
}