#version 410

out vec4 FragColor;

in vec2 TexCoords;  // texture coordinates of the quad

uniform samplerCube cubemap;
uniform bool mode;  // true = unfolded. False = equirectangular

#define PI 3.1415926538

/**
 * This shader draw:
 * - Either an unfolded cubemap onto a 2D texture
 * - Or a equirectangular projection
 */

void render_equirectangular_projection() {
    float phi = TexCoords.x * PI * 2;
    float theta = (-TexCoords.y + 0.5) * PI;
    vec3 dir = vec3(cos(phi) * cos(theta), sin(theta), sin(phi) * cos(theta));

    FragColor = texture(cubemap, dir);
}

void render_unfolded() {
    // The resulting quad is divided into 4 columns and 3 rows.
    const int number_of_columns = 4;
    const int number_of_rows = 3;

    int current_column = int(TexCoords.x * float(number_of_columns));
    int current_row = int((TexCoords.y) * float(number_of_rows));

    vec2 texture_coordinates_per_side = TexCoords;
    texture_coordinates_per_side.x = mod(texture_coordinates_per_side.x * 4, 1);
    texture_coordinates_per_side.y = mod(texture_coordinates_per_side.y * 3, 1);

    bool should_sample = false;
    vec3 sample_direction;

    if (current_column == 0 && current_row == 1) {
        // draw left
        should_sample = true;

        sample_direction =
            vec3(-1., -texture_coordinates_per_side.y * 2. + 1., texture_coordinates_per_side.x * 2. - 1.);
    }
    if (current_column == 1 && current_row == 0) {
        // draw top
        should_sample = true;

        sample_direction = vec3(texture_coordinates_per_side.x * 2. - 1., 1., texture_coordinates_per_side.y * 2. - 1.);
    }
    if (current_column == 1 && current_row == 1) {
        // draw front
        should_sample = true;

        sample_direction = vec3(texture_coordinates_per_side.x * 2. - 1., -texture_coordinates_per_side.t * 2. + 1., 1);
    }
    if (current_column == 1 && current_row == 2) {
        // draw bottom
        should_sample = true;

        sample_direction =
            vec3(texture_coordinates_per_side.x * 2. - 1., -1., texture_coordinates_per_side.y * -2. + 1.);
    }
    if (current_column == 2 && current_row == 1) {
        // draw right
        should_sample = true;

        sample_direction =
            vec3(1., -texture_coordinates_per_side.y * 2. + 1., -texture_coordinates_per_side.x * 2. + 1.);
    }
    if (current_column == 3 && current_row == 1) {
        // draw back
        should_sample = true;

        sample_direction =
            vec3(-texture_coordinates_per_side.x * 2. + 1., -texture_coordinates_per_side.y * 2. + 1., -1.);
    }

    if (should_sample) {
        FragColor = texture(cubemap, sample_direction);
    } else {
        FragColor = vec4(0., 0., 0., 1.);
    }
}

void main() {
    if (!mode) {
        render_equirectangular_projection();
        return;
    }

    render_unfolded();
    return;
}