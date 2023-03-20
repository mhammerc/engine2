#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadow_matrices[6];

out vec4 FragPos;

/**
 * We are drawing to a cubemap. So we want to draw the scene 6 times for the 6 faces of the cubemap.
 *
 * Our main() function takes three vertex in: a triangle. `layout (triangles) in;`
 *
 * Our main() function transform this 3 vertex to 18 vertex: 6 triangles of 3 vertex each = 18 vertex. `layout(triangle_strip, max_vertices = 18) out;`
 *
 * 6 output triangles because we have 6 face therefore we want to render the scene 6 times.
 *
 * For each outputted triangle, we specify which face to draw on: `gl_Layer = face`.
 *
 * This means that each triangle will be drawn to a different face.
 */
void main() {
    for (int face = 0; face < 6; ++face) {
        gl_Layer = face;
        mat4 shadow_matrix = shadow_matrices[face];

        for (int i = 0; i < 3; ++i)  // for each vertex of the triangle
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadow_matrix * FragPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}