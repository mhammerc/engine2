#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelNormal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragmentPosition = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;

    Normal = modelNormal * aNormal;
}