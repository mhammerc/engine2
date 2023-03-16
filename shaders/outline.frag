#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D outline_texture;

// Lookups in all 8 diagonals directions, up to a distance of two
const ivec2 offsets[24] = ivec2[](
    ivec2(0, 1),
    ivec2(0, -1),
    ivec2(1, 0),
    ivec2(-1, 0),
    ivec2(1, 1),
    ivec2(-1, 1),
    ivec2(1, -1),
    ivec2(-1, -1),

    ivec2(0, 2),
    ivec2(0, -2),
    ivec2(1, 2),
    ivec2(1, -2),
    ivec2(-1, 2),
    ivec2(-1, -2),
    ivec2(2, 2),
    ivec2(2, -2),
    ivec2(-2, 2),
    ivec2(-2, -2),

    ivec2(2, 0),
    ivec2(-2, 0),
    ivec2(2, 1),
    ivec2(-2, 1),
    ivec2(2, -1),
    ivec2(-2, -1)
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
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[8]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[9]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[10]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[11]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[12]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[13]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[14]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[15]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[16]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[17]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[18]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[19]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[20]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[21]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[22]).r;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[23]).r;

    alpha = min(alpha, 1.) * on_object;

    // The same orange outline as blender
    vec3 color = vec3(0xF8, 0x9B, 0x32) / 255;
    FragColor = vec4(color, alpha);
}