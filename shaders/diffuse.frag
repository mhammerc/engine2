#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

#define LIGHT_TYPE_UNSET 0x0000
#define LIGHT_TYPE_DIRECTIONAL 0x0001
#define LIGHT_TYPE_POINT 0x0002
#define LIGHT_TYPE_SPOT 0x0003

struct Light {
    int type;

    vec3 position; // point, spot
    vec3 direction; // directional, spot

    float innerCutOff; // spot
    float outerCutOff; // spot
    float constant; // spot
    float linear; // spot
    float quadratic; // spot

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NUMBER_OF_LIGHTS 10
uniform Light lights[NUMBER_OF_LIGHTS];

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

vec3 lightDirectional(Light light, vec3 normal, vec3 viewDir);
vec3 lightPoint(Light light, vec3 normal, vec3 viewDir);
vec3 lightSpot(Light light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 color = vec3(0);

    for (int i = 0; i < NUMBER_OF_LIGHTS; ++i) {
        if (lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
            color += lightDirectional(lights[i], normal, viewDir);
        }
        else if (lights[i].type == LIGHT_TYPE_POINT) {
            color += lightPoint(lights[i], normal, viewDir);
        }
        else if (lights[i].type == LIGHT_TYPE_SPOT) {
            color += lightSpot(lights[i], normal, viewDir);
        }
    }

    FragColor = vec4(color, 1.);
}

vec3 lightDirectional(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(- light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 lightPoint(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 lightSpot(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(- light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}