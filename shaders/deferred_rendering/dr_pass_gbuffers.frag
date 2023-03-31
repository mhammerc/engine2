#version 410 core

layout(location = 0) out vec3 g_albedo;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec3 g_specular_outline;
layout(location = 3) out vec4 g_identify;

in VS_OUT {
    vec2 fragment_uv;
    vec3 fragment_normal_world;
    vec3 fragment_position_world;
}
vs_out;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform int identity;
uniform bool is_outline;

void main() {
    g_albedo = texture(texture_diffuse1, vs_out.fragment_uv).rgb;

    g_normal = normalize(vs_out.fragment_normal_world);

    float specular = texture(texture_specular1, vs_out.fragment_uv).r;
    float outline = float(is_outline);
    g_specular_outline = vec3(specular, outline, 0.);

    g_identify = vec4(identity, 0., 0., 1.);
}