#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragmentPosition;
}
vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelNormal;

void main() {
    vs_out.FragmentPosition = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * vec4(vs_out.FragmentPosition, 1.0);

    vs_out.TexCoord = aTexCoord;

    vs_out.Normal = modelNormal * aNormal;
}