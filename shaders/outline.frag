#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

// The outline information is in the G channel.
uniform sampler2D outline_texture;

// Lookups in all 8 diagonals directions, up to a distance of two
const ivec2 offsets[48] = ivec2[](
    // Distance 1
    ivec2(0, 1),
    ivec2(1, 1),
    ivec2(-1, 1),

    ivec2(0, -1),
    ivec2(1, -1),
    ivec2(-1, -1),

    ivec2(1, 0),
    ivec2(-1, 0),

    // Distance 2
    ivec2(0, 2),
    ivec2(1, 2),
    ivec2(-1, 2),
    ivec2(2, 2),
    ivec2(-2, 2),

    ivec2(0, -2),
    ivec2(1, -2),
    ivec2(-1, -2),
    ivec2(2, -2),
    ivec2(-2, -2),

    ivec2(2, 0),
    ivec2(2, 1),
    ivec2(2, -1),

    ivec2(-2, 0),
    ivec2(-2, 1),
    ivec2(-2, -1),

    // Distance 3
    ivec2(0, 3),
    ivec2(1, 3),
    ivec2(2, 3),
    ivec2(3, 3),
    ivec2(-1, 3),
    ivec2(-2, 3),
    ivec2(-3, 3),

    ivec2(0, -3),
    ivec2(1, -3),
    ivec2(2, -3),
    ivec2(3, -3),
    ivec2(-1, -3),
    ivec2(-2, -3),
    ivec2(-3, -3),

    ivec2(3, 0),
    ivec2(3, 1),
    ivec2(3, 2),
    ivec2(3, -1),
    ivec2(3, -2),
    ivec2(-3, 0),
    ivec2(-3, 1),
    ivec2(-3, 2),
    ivec2(-3, -1),
    ivec2(-3, -2)
);

void main() {
    // We want to draw the outline only if:
    //  - Current pixel is on the object to be outlined.
    //  - One of the neighboring pixels is not on the object.
    //    We can check this using 8 textures lookups: top, bottom, left, right and diagonals
    //    If we want the line to be thicker, we will need more than 8 textures lookup.

    float on_object = texture(outline_texture, TexCoords).g;

    float alpha = 0.;

    // Can't do a for loop as it looks like to never be unrolled.
    // (macOS)
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[0]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[1]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[2]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[3]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[4]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[5]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[6]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[7]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[8]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[9]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[10]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[11]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[12]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[13]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[14]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[15]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[16]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[17]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[18]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[19]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[20]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[21]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[22]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[23]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[24]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[25]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[26]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[27]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[28]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[29]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[30]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[31]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[32]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[33]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[34]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[35]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[36]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[37]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[38]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[39]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[40]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[41]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[42]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[43]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[44]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[45]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[46]).g;
    alpha += 1. - textureOffset(outline_texture, TexCoords, offsets[47]).g;

    alpha = min(alpha, 1.) * on_object;

    // The same orange outline as blender
    vec3 color = vec3(0xF8, 0x9B, 0x32) / 255;
    FragColor = vec4(color, alpha);
}