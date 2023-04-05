#version 410 core

out vec4 FragColor;
in vec2 uv_coords;

uniform sampler2D tex_to_downsample;
uniform vec2 tex_to_downsample_size;  // TODO: check wording with code

/**
 * This downsample method comes from Léna Piquet:
 * https://www.froyok.fr/blog/2021-12-ue4-custom-bloom/
 *
 * It is simple: 13 weighted samples compute the downsampled pixel.
 */

const vec2 coords[13] = vec2[](
    // distance: 0
    vec2(0.0, 0.0),

    // distance: 1
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),

    // distance: 2
    vec2(-2.0, 2.0),
    vec2(0.0, 2.0),
    vec2(2.0, 2.0),
    vec2(-2.0, 0.0),
    vec2(2.0, 0.0),
    vec2(-2.0, -2.0),
    vec2(0.0, -2.0),
    vec2(2.0, -2.0)
);

const float weights[13] = float[](
    // 1 sample at distance 0 (same as distance 2)
    0.0555555,

    // 4 samples at distance 1
    // (1 / 4) * 0.5f = 0.125f
    0.125,
    0.125,
    0.125,
    0.125,

    // 8 samples at distance 2
    // (1 / 9) * 0.5f
    0.0555555,
    0.0555555,
    0.0555555,
    0.0555555,
    0.0555555,
    0.0555555,
    0.0555555,
    0.0555555
);

vec3 downsample() {
    vec2 pixel_size = (1. / tex_to_downsample_size) * 0.5;
    vec3 result = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < 13; ++i) {
        vec2 uv = uv_coords + coords[i] * pixel_size;
        result += weights[i] * clamp(texture(tex_to_downsample, uv).rgb, 0., 2000.);
    }

    return result;
}

void main() {
    FragColor.rgb = downsample();
    FragColor.a = 1.;
}