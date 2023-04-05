#version 410 core
#include "light.glsl"

out vec4 FragColor;
in vec2 fragment_uv;

uniform sampler2D g_albedo;
uniform sampler2D g_normal_world;
uniform sampler2D g_specular;
uniform sampler2D g_depth;

uniform vec3 camera_position;
uniform mat4 projection_matrix_inverse;
uniform mat4 view_matrix_inverse;

vec3 position_world = vec3(0, 0, -1);  // set later
vec3 fragment_to_camera_direction = vec3(1, 0, 0);  // set later
vec3 normal_world = texture(g_normal_world, fragment_uv).xyz;
vec3 albedo = texture(g_albedo, fragment_uv).rgb;
float specular = texture(g_specular, fragment_uv).r;
float depth_projection = texture(g_depth, fragment_uv).r;

vec3 lightDirectional(Light light);
vec3 lightPoint(Light light);
vec3 lightSpot(Light light);

vec3 world_position_from_depth() {
    float depth_ndc = depth_projection * 2. - 1.;
    vec2 fragment_uv_ndc = fragment_uv * 2. - 1.;

    // reminder: clip space is always ndc.
    vec4 position_clip_space = vec4(fragment_uv_ndc, depth_ndc, 1.0);
    vec4 position_view_space = projection_matrix_inverse * position_clip_space;

    position_view_space /= position_view_space.w;

    vec4 position_world_space = view_matrix_inverse * position_view_space;

    return position_world_space.xyz;
}

void main() {
    position_world = world_position_from_depth();
    fragment_to_camera_direction = normalize(camera_position - position_world);

    vec3 fragColor = vec3(0);

    for (int i = 0; i < /*NUMBER_OF_LIGHTS*/ 1; ++i) {
        Light light = lights[i];

        light.direction = normalize(light.direction);

        if (lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
            fragColor += lightDirectional(light);
        } else if (lights[i].type == LIGHT_TYPE_POINT) {
            fragColor += lightPoint(light);
        } else if (lights[i].type == LIGHT_TYPE_SPOT) {
            fragColor += lightSpot(light);
        }
    }

    FragColor = vec4(fragColor, 1.0);
}

float shadow_calculation(Light light) {
    // FragmentPosition
    vec3 light_to_fragment = position_world - light.position;
    float closest_depth = texture(shadow_map, light_to_fragment).r;
    closest_depth *= shadow_map_far_plane;

    float current_depth = length(light_to_fragment);

    if (current_depth >= shadow_map_far_plane) {
        // Too far away from the light caster
        return 0.;
    }

    float bias = 0.05;
    float shadow = current_depth - bias > closest_depth ? 1. : 0.;

    return shadow;
}

vec3 lightDirectional(Light light) {
    // Direction from fragment towards light.
    vec3 fragment_to_light_direction = -light.direction;

    // diffuse shading
    float diffuse_intensity = max(dot(normal_world, fragment_to_light_direction), 0.0);

    // specular shading
    vec3 halfway_direction = normalize(fragment_to_light_direction + fragment_to_camera_direction);
    float specular_intensity = pow(max(dot(normal_world, halfway_direction), 0.0), 10 /*material.shininess*/);

    // combine results
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diffuse_intensity * albedo;
    vec3 specular = light.specular * specular_intensity * (specular / 2.);

    return (ambient + diffuse + specular);
}

vec3 lightPoint(Light light) {
    // A point light have a position and emit light in all direction.
    // Therefore, compute the direction from the fragment to the light.
    vec3 fragment_to_light_direction = normalize(light.position - position_world);

    // diffuse shading
    float diffuse_intensity = max(dot(normal_world, fragment_to_light_direction), 0.0);

    // specular shading
    vec3 halfway_direction = normalize(fragment_to_light_direction + fragment_to_camera_direction);
    float specular_intensity = pow(max(dot(normal_world, halfway_direction), 0.0), 10 /*material.shininess*/);

    // attenuation
    // It is a simple quadratic function
    // (eg. `a + bx + cxx` where x is distance between fragment and distance).
    float fragment_light_distance = length(light.position - position_world);
    float attenuation = 1.0
        / (light.constant + light.linear * fragment_light_distance
           + light.quadratic * (fragment_light_distance * fragment_light_distance));

    // combine results
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diffuse_intensity * albedo;
    vec3 specular = light.specular * specular_intensity * specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = shadow_calculation(light);

    // return (ambient + diffuse + specular);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 lightSpot(Light light) {
    vec3 fragment_to_light_direction = normalize(light.position - position_world);
    vec3 lightToFragmentDirection = -fragment_to_light_direction;

    // diffuse shading
    float diffuse_intensity = max(dot(normal_world, fragment_to_light_direction), 0.0);

    // specular shading
    vec3 halfway_direction = normalize(fragment_to_light_direction + fragment_to_camera_direction);
    float specular_intensity = pow(max(dot(normal_world, halfway_direction), 0.0), 10 /*material.shininess*/);

    // attenuation
    float distance = length(light.position - position_world);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(fragment_to_light_direction, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diffuse_intensity * albedo;
    vec3 specular = light.specular * specular_intensity * (specular / 2.);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}