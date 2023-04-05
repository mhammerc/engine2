#version 410 core

#include "tone_mapping_aces.glsl"

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screen_texture;

uniform float exposure;
uniform int tone_mapping;

uniform bool bloom_enabled;
uniform sampler2D bloom_texture;
uniform float bloom_intensity;

// uniform int post_process;

// #define POST_PROCESS_INVERSE (1 << 0)
// #define POST_PROCESS_GRAYSCALE (1 << 1)
// #define POST_PROCESS_SEPIA (1 << 2)
// #define POST_PROCESS_BLUR (1 << 3)
// #define POST_PROCESS_SHARPEN (1 << 4)
// #define POST_PROCESS_EDGE_DETECTION (1 << 5)

// // 2560 1800
// const float offsetX = 1.0 / 2560.0;
// const float offsetY = 1.0 / 1800.0;

// vec2 offsets[9] = vec2[](
//     vec2(-offsetX, offsetY),  // top-left
//     vec2(0.0f, offsetY),  // top-center
//     vec2(offsetX, offsetY),  // top-right
//     vec2(-offsetX, 0.0f),  // center-left
//     vec2(0.0f, 0.0f),  // center-center
//     vec2(offsetX, 0.0f),  // center-right
//     vec2(-offsetX, -offsetY),  // bottom-left
//     vec2(0.0f, -offsetY),  // bottom-center
//     vec2(offsetX, -offsetY)  // bottom-right
// );

// float sharpen_kernel[9] = float[](-1, -1, -1, -1, 9, -1, -1, -1, -1);

// float blur_kernel[9] =
//     float[](1.0 / 16., 2.0 / 16., 1.0 / 16., 2.0 / 16., 4.0 / 16., 2.0 / 16., 1.0 / 16., 2.0 / 16., 1.0 / 16.);

// float edge_detection_kernel[9] = float[](1., 1., 1., 1., -8., 1., 1., 1., 1.);

// vec3 compute_pixel(vec3 pixel) {
//     vec3 result = pixel;

//     if ((post_process & POST_PROCESS_INVERSE) != 0) {
//         result = vec3(1.0 - result);
//     }

//     if ((post_process & POST_PROCESS_GRAYSCALE) != 0) {
//         float average = result.x * 0.2126 + result.y * 0.7152 + result.z * 0.0722;
//         result = vec3(average);
//     }

//     if ((post_process & POST_PROCESS_SEPIA) != 0) {
//         pixel = result;
//         result.r = 0.393 * pixel.r + 0.769 * pixel.g + 0.189 * pixel.b;
//         result.g = 0.349 * pixel.r + 0.686 * pixel.g + 0.168 * pixel.b;
//         result.b = 0.272 * pixel.r + 0.534 * pixel.g + 0.131 * pixel.b;
//     }

//     return result;
// }

// vec3 compute_kernel(float kernel[9]) {
//     vec3 sampleTex[9];
//     for (int i = 0; i < 9; i++) {
//         sampleTex[i] = vec3(texture(screen_texture, TexCoords.st + offsets[i]));
//         sampleTex[i] = compute_pixel(sampleTex[i]);
//     }

//     vec3 col = vec3(0.0);
//     for (int i = 0; i < 9; i++)
//         col += sampleTex[i] * kernel[i];

//     return col;
// }

void main() {
    // This snippet visualize all "HDR" values (above 1.0) as red.
    // vec4 fragmentTexture = texture(screen_texture, TexCoords);
    // if (fragmentTexture.r > 1. || fragmentTexture.g > 1. || fragmentTexture.b > 1.) {
    //     FragColor = vec4(1., 0., 0., 1.);
    //     return;
    // }

    // FragColor = texture(screen_texture, TexCoords);

    // if ((post_process & POST_PROCESS_SHARPEN) != 0) {
    //     FragColor = vec4(compute_kernel(sharpen_kernel), 1.);
    // } else if ((post_process & POST_PROCESS_BLUR) != 0) {
    //     FragColor = vec4(compute_kernel(blur_kernel), 1.);
    // } else if ((post_process & POST_PROCESS_EDGE_DETECTION) != 0) {
    //     FragColor = vec4(compute_kernel(edge_detection_kernel), 1.);
    // } else {
    //     FragColor = vec4(compute_pixel(FragColor.xyz), 1.);
    // }

    // float exposure = 1.0;

    vec3 hdr_color = texture(screen_texture, TexCoords).rgb;

    if (bloom_enabled) {
        hdr_color += texture(bloom_texture, TexCoords).rgb * bloom_intensity;
    }

    if (tone_mapping == 0x01) {
        // more precise but more expensive aces

        FragColor.rgb = aces_fitted(hdr_color * exposure);
    } else if (tone_mapping == 0x02) {
        // faster and more saturated aces

        FragColor.rgb = aces_approx(hdr_color * exposure);
    } else {
        // No tone mapping

        FragColor.rgb = hdr_color * exposure;
    }

    const float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1. / gamma));

    FragColor.a = 1.;
}