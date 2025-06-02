#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

in vec3 worldPosition;
in vec3 worldNormal;
in vec4 worldTangent;
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

#define MATERIAL_BUFFER_ID 0

struct Material
{
    uint textures[MapCount];
};

layout(std430, binding = MATERIAL_BUFFER_ID) buffer MaterialBuffer
{
    Material materials[];
};

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

struct PointLight
{
    vec3 position;
    vec3 color;
};

#define LIGHT_LIMIT 10
uniform PointLight lights[LIGHT_LIMIT];
uniform uint lightCount;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 GetNormal(Material material)
{
    vec3 T = normalize(worldTangent.xyz);
    vec3 N = normalize(worldNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * worldTangent.w;
    mat3 TBN = mat3(T, B, N);
    vec3 normalTangent = texture(sampler2D(material.textures[MapNormal]), fragTexCoords).rgb;
    normalTangent = normalize(normalTangent * 2.0 - 1.0);
    N = normalize(TBN * normalTangent);
    return N;
}

void main()
{
    Material material = materials[fragMaterialIndex];

    vec3 N = GetNormal(material);
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 R = reflect(-V, N);

    vec3 albedo = texture(sampler2D(material.textures[MapDiffuse]), fragTexCoords).rgb;
    vec3 rma = texture(sampler2D(material.textures[MapRMA]), fragTexCoords).rgb;

    float roughness = rma.r;
    float metallic = rma.g;
    float ao = rma.b;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightCount; ++i)
    {
        vec3 L = normalize(lights[i].position - worldPosition);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - worldPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    fragColor = vec4(color, 1.0);
}

