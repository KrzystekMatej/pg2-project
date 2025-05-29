#version 460 core
#extension GL_ARB_bindless_texture : require

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
    uvec2 textures[MapCount];
};

layout(std430, binding = MATERIAL_BUFFER_ID) buffer MaterialBuffer
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

const float PI = 3.14159265359;

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

void main(void)
{
    Material material = materials[fragMaterialIndex];

    vec3 T = normalize(worldTangent.xyz);
    vec3 N = normalize(worldNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * worldTangent.w;
    mat3 TBN = mat3(T, B, N);
    sampler2D normalMap = sampler2D(material.textures[MapNormal]);
    vec3 normalTangent = texture(normalMap, fragTexCoord).rgb;
    
   

    vec3 normalColor = normalize(worldNormal) * 0.5 + 0.5;
    fragColor = vec4(normalColor, 1.0);
}
