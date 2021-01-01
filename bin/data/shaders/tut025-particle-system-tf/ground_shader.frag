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

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;
uniform PointLight firePointLight;
uniform FogParameters fogParams;

uniform sampler2D snowSampler;
uniform sampler2D pathSampler;
uniform sampler2D pavementSampler;

void main()
{
    vec3 normal = normalize(ioVertexNormal);

    vec4 snowTexel = texture(snowSampler, ioVertexTexCoord);
    vec4 pathTexel = texture(pathSampler, ioVertexTexCoord / 20.0f);
    vec4 pavementTexel = texture(pavementSampler, ioVertexTexCoord);
    float pathWeight = pathTexel.r;
    float snowWeight = 1.0 - pathWeight;
    vec4 groundColor = pavementTexel*pathWeight + snowTexel*snowWeight;

    vec3 lightColor = getAmbientLightColor(ambientLight) + getDiffuseLightColor(diffuseLight, normal) + getPointLightColor(firePointLight, ioWorldPosition.xyz, normal);

    outputColor = groundColor*vec4(lightColor, 1.0);
    
    // Apply fog calculation only if fog is enabled
    if(fogParams.isEnabled)
    {
        float fogCoordinate = abs(ioEyeSpacePosition.z / ioEyeSpacePosition.w);
        outputColor = mix(outputColor, vec4(fogParams.color, 1.0), getFogFactor(fogParams, fogCoordinate));
    }
}