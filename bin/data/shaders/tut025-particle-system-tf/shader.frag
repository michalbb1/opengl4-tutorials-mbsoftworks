#version 440 core

#include "../lighting/ambientLight.frag"
#include "../lighting/diffuseLight.frag"
#include "../lighting/pointLight.frag"
#include "../fog/fog.frag"

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;
smooth in vec3 ioVertexNormal;
smooth in vec4 ioWorldPosition;
smooth in vec4 ioEyeSpacePosition;

uniform sampler2D sampler;
uniform vec4 color;

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;
uniform PointLight firePointLight;

uniform FogParameters fogParams;
uniform vec3 eyePosition;

uniform int numPointLights;
layout(std140, binding = 1) uniform PointLightsBlock
{
    PointLight lights[150];
} block_pointLights;

void main()
{
    vec3 normal = normalize(ioVertexNormal);
    vec4 textureColor = texture(sampler, ioVertexTexCoord);
    vec4 objectColor = textureColor*color;
    vec3 ambientColor = getAmbientLightColor(ambientLight);
    vec3 diffuseColor = getDiffuseLightColor(diffuseLight, normal);
	vec3 lightColor = ambientColor + diffuseColor + getPointLightColor(firePointLight, ioWorldPosition.xyz, normal);

    outputColor = objectColor * vec4(lightColor, 1.0);

    // Apply fog calculation only if fog is enabled
    if(fogParams.isEnabled)
    {
        float fogCoordinate = abs(ioEyeSpacePosition.z / ioEyeSpacePosition.w);
        outputColor = mix(outputColor, vec4(fogParams.color, 1.0), getFogFactor(fogParams, fogCoordinate));
    }
}