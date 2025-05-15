#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

in vec3 worldPosition;
in vec3 worldNormal;
in vec3 worldTangent;
in vec2 fragTexCoords;
flat in uint fragMaterialIndex;

out vec4 fragColor;

//texture map enum
const int MapDiffuse = 0;
const int MapSpecular = 1;
const int MapGlossiness = 2;
const int MapRMA = 3;
const int MapNormal = 4;
const int MapOpacity = 5;
const int MapEmission = 6;
const int MapCount = 7;

uniform vec3 cameraPosition;

struct Material
{
    uint64_t textures[MapCount];
};

layout(std430, binding = 0) buffer MaterialBuffer
{
    Material materials[];
};

struct PointLight
{
    vec3 position;
    vec3 color;
};

#define LIGHT_LIMIT 10
uniform PointLight lights[LIGHT_LIMIT];
uniform uint lightCount;

const float pi = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec3 T = normalize(worldTangent.xyz);
    vec3 N = normalize(worldNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * worldTangent.w;
    TBN = mat3(T, B, N);
    vec3 normalTangent = texture(sampler2D(material.textures[MapNormal]), fragTexCoord).rgb;
    normalTangent = normalize(normalTangent * 2.0 - 1.0);
    vec3 N = normalize(TBN * normalTangent);

    vec3 V = normalize(cameraPosition - worldPosition);


    Material material = materials[fragMaterialIndex];

    vec3 albedo = texture(sampler2D(material.textures[MapDiffuse]), fragTexCoord).rgb;
    vec3 rma = texture(sampler2D(material.textures[MapRMA]), fragTexCoord).rgb;

    float roughness = rma.r;
    float metallic = rma.g;
    float ao = rma.b;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightCount; ++i)
    {
        vec3 L = normalize(lights[i].position - WorldPosition);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - WorldPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

