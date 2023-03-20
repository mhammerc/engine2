#version 410 core

out vec4 FragColor;

in GEO_OUT {
    vec2 TexCoord;
    vec3 Normal;  // world position
    vec3 FragmentPosition;  // world position
}
vs_out;

vec2 TexCoord = vs_out.TexCoord;
vec3 Normal = vs_out.Normal;
vec3 FragmentPosition = vs_out.FragmentPosition;

// Position of the camera
uniform vec3 cameraPosition;

#define LIGHT_TYPE_UNSET 0x0000
#define LIGHT_TYPE_DIRECTIONAL 0x0001
#define LIGHT_TYPE_POINT 0x0002
#define LIGHT_TYPE_SPOT 0x0003

/**
 * `struct Light` can represent any light supported by the engine.
 *
 * At the moment we support only 10 lights in a shader ; that means the engine
 * must pick the 10 closest lights and give then to the shader through uniforms
 * before drawing.
 *
 * `LIGHT_TYPE_DIRECTIONAL`: Represent a light infinitely far away, thus
 * lighting from a given direction without attenuation. Like the sun.
 *
 * `LIGHT_TYPE_POINT`: Represent a light in a given position, emitting light in
 * all directions. It is affected by attenuation.
 *
 * `LIGHT_TYPE_SPOT`: Represent a light in a given position and a given
 * direction. Emits light towards the direction from the position, in a circle
 * with a given angle. See innerCutOff and outerCutOff.
 *
 * `LIGHT_TYPE_UNSET`: The light is not a light. Do nothing.
 */
struct Light {
    // See above for types (eg. LIGHT_TYPE_XXX)
    int type;

    vec3 position;  // point, spot
    vec3 direction;  // directional, spot. Direction where the light points.

    // Spot lights inner and outer cutoff for a smooth transition
    float innerCutOff;  // spot
    float outerCutOff;  // spot

    // constant, linear and quadratic components for attenuation
    float constant;  // spot
    float linear;  // spot
    float quadratic;  // spot

    // What is the color of the light?
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform samplerCube shadow_map;
uniform float shadow_map_far_plane;

#define NUMBER_OF_LIGHTS 10
uniform Light lights[NUMBER_OF_LIGHTS];

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;

    sampler2D texture_specular1;
    sampler2D texture_specular2;

    samplerCube texture_environment;

    // How much specular components
    float shininess;
    float reflect;
    float refract;
};

uniform Material material;

vec3 lightDirectional(Light light, vec3 normal, vec3 fragmentToCameraDirection);
vec3 lightPoint(Light light, vec3 normal, vec3 fragmentToCameraDirection);
vec3 lightSpot(Light light, vec3 normal, vec3 fragmentToCameraDirection);
float shadow_calculation(Light light);

void main() {
    // The normal vector comes from the vertex shader. Because of interpolation,
    // we must normalize it.
    vec3 normal = normalize(Normal);

    vec3 fragmentToCameraDirection = normalize(cameraPosition - FragmentPosition);

    vec3 fragColor = vec3(0);

    if (material.reflect != 0.) {
        vec3 reflection = reflect(-fragmentToCameraDirection, normal);
        fragColor += texture(material.texture_environment, reflection).xyz;
    } else if (material.refract != 0.) {
        float ratio = 1. / 1.52;
        vec3 refraction = refract(-fragmentToCameraDirection, normal, ratio);
        fragColor += texture(material.texture_environment, refraction).xyz;
    } else {
        for (int i = 0; i < /*NUMBER_OF_LIGHTS*/ 1; ++i) {
            Light light = lights[i];

            light.direction = normalize(light.direction);

            if (lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
                fragColor += lightDirectional(light, normal, fragmentToCameraDirection);
            } else if (lights[i].type == LIGHT_TYPE_POINT) {
                fragColor += lightPoint(light, normal, fragmentToCameraDirection);
            } else if (lights[i].type == LIGHT_TYPE_SPOT) {
                fragColor += lightSpot(light, normal, fragmentToCameraDirection);
            }
        }
    }

    FragColor = vec4(fragColor, 1.);
}

float shadow_calculation(Light light) {
    // FragmentPosition
    vec3 light_to_fragment = FragmentPosition - light.position;
    float closest_depth = texture(shadow_map, light_to_fragment).r;
    closest_depth *= shadow_map_far_plane;

    float current_depth = length(light_to_fragment);

    float bias = 0.05;
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

    return shadow;
}

vec3 lightDirectional(Light light, vec3 normal, vec3 fragmentToCameraDirection) {
    // Direction from fragment towards light.
    vec3 fragmentToLightDirection = -light.direction;

    // diffuse shading
    float diffuseIntensity = max(dot(normal, fragmentToLightDirection), 0.0);

    // specular shading
    vec3 halfwayDirection = normalize(fragmentToLightDirection + fragmentToCameraDirection);
    float specularIntensity = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * specularIntensity * (vec3(texture(material.texture_specular1, TexCoord)) / 2.);

    return (ambient + diffuse + specular);
}

vec3 lightPoint(Light light, vec3 normal, vec3 fragmentToCameraDirection) {
    // A point light have a position and emit light in all direction.
    // Therefore, compute the direction from the fragment to the light.
    vec3 fragmentToLightDirection = normalize(light.position - FragmentPosition);

    // diffuse shading
    float diffuseIntensity = max(dot(normal, fragmentToLightDirection), 0.0);

    // specular shading
    vec3 halfwayDirection = normalize(fragmentToLightDirection + fragmentToCameraDirection);
    float specularIntensity = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    // attenuation
    // It is a simple quadratic function
    // (eg. `a + bx + cxx` where x is distance between fragment and distance).
    float fragmentLightDistance = length(light.position - FragmentPosition);
    float attenuation = 1.0
        / (light.constant + light.linear * fragmentLightDistance
           + light.quadratic * (fragmentLightDistance * fragmentLightDistance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * specularIntensity * (vec3(texture(material.texture_specular1, TexCoord)) / 2.);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = shadow_calculation(light);

    // return (ambient + diffuse + specular);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 lightSpot(Light light, vec3 normal, vec3 fragmentToCameraDirection) {
    vec3 fragmentToLightDirection = normalize(light.position - FragmentPosition);
    vec3 lightToFragmentDirection = -fragmentToLightDirection;

    // diffuse shading
    float diffuseIntensity = max(dot(normal, fragmentToLightDirection), 0.0);

    // specular shading
    vec3 halfwayDirection = normalize(fragmentToLightDirection + fragmentToCameraDirection);
    float specularIntensity = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - FragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(fragmentToLightDirection, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * specularIntensity * (vec3(texture(material.texture_specular1, TexCoord)) / 2.);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}