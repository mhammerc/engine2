#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelNormal;

void main()
{
    vec3 Normal = modelNormal * aNormal;

    vec3 FragmentPosition = vec3(model * vec4(aPos, 1.0));

    // To make the outline, we "scale up" the vertex slightly toward the Normal.
    // This way of doing works quite well but will still have visual bugs where the outline
    // is missing.
    gl_Position = projection * view * vec4(FragmentPosition + Normal * 0.05, 1.0);

    vec2 TexCoord = aTexCoord;
}