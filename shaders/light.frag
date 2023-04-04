#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 color;

void main() {
    FragColor = vec4(color * 3, 1.);
}