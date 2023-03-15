#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D outline_texture;

// Lookups in all 8 diagonals directions
const ivec2 offsets[8] = ivec2[](
    ivec2(0, 1),
    ivec2(0, -1),
    ivec2(1, 0),
    ivec2(-1, 0),
    ivec2(1, 1),
    ivec2(-1, 1),
    ivec2(1, -1),
    ivec2(-1, -1)
);

void main() {
    // We want to draw the outline only if:
    //  - Current pixel is on the object to be outlined.
    //  - One of the neighboring pixels is not on the object.
    //    We can check this using 8 textures lookups: top, bottom, left, right and diagonals

    float on_object = texture(outline_texture, TexCoords).r;

    float alpha = 0.;

    // Can't do a for loop as it looks like to never be unrolled.
    // (macOS)
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[0]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[1]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[2]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[3]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[4]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[5]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[6]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[7]).r;

    alpha = min(alpha, 1.) * on_object;

    // The same orange outline as blender
    vec3 color = vec3(0xF8, 0x9B, 0x32) / 255;
    FragColor = vec4(color, alpha);
}