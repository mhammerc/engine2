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
