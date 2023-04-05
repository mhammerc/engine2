#version 410 core

out vec4 FragColor;
in vec2 uv_coords;

uniform sampler2D tex_to_upsample;

uniform sampler2D tex_current;
uniform vec2 tex_current_size;

uniform float radius;

/**
 * This upsample method comes from Léna Piquet:
 * https://www.froyok.fr/blog/2021-12-ue4-custom-bloom/
 *
 * It is simple: 9 weighted samples compute the upsampled pixels.
 */

const vec2 coords[9] = vec2[](
    vec2(-1.0, 1.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, 0.0),
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(-1.0, -1.0),
    vec2(0.0, -1.0),
    vec2(1.0, -1.0)
);

const float weights[9] = float[](0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625);

vec3 upsample() {
    vec2 pixel_size = 1. / tex_current_size;
    vec3 result = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < 9; ++i) {
        vec2 uv = uv_coords + coords[i] * pixel_size;
        result += weights[i] * texture(tex_to_upsample, uv).rgb;
    }

    return result;
}

void main() {
    vec3 current_color = texture(tex_current, uv_coords).rgb;
    vec3 upsampled_color = upsample();

    FragColor.rgb = mix(current_color, upsampled_color, radius);
    FragColor.a = 1.;
}