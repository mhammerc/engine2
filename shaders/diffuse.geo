#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragmentPosition;
} vs_out[3];

out GEO_OUT {
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragmentPosition;
} geo_out;

uniform float explosion;

void copy_vs_to_geo(int index) {
    geo_out.TexCoord = vs_out[index].TexCoord;
    geo_out.Normal = vs_out[index].Normal;
    geo_out.FragmentPosition = vs_out[index].FragmentPosition;
}

vec3 get_normal()
{
    // To compute the normal in NDC:
    // We have 3 vertices. So, we can take theses 3 vertices as a plane,
    // and compute the direction perpendicular to the plane.
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void vertex_for_index(int index, vec3 normal) {
    float magnitude = 2.;
    vec3 direction = normal * explosion *magnitude;
    gl_Position = gl_in[index].gl_Position + vec4(direction, 0.);

    copy_vs_to_geo(index);

    EmitVertex();
}

void main() {
    vec3 normal = get_normal();

    vertex_for_index(0, normal);
    vertex_for_index(1, normal);
    vertex_for_index(2, normal);

    EndPrimitive();
}