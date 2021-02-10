#version 440 core

#include "../lighting/ambientLight.frag"
#include "../lighting/diffuseLight.frag"
#include "../fog/fog.frag"

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;
smooth in vec3 ioVertexNormal;
smooth in vec4 ioEyeSpacePosition;

uniform sampler2D sampler;
uniform vec4 color;

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;
uniform FogParameters fogParams;

void main()
{
    vec3 normal = normalize(ioVertexNormal);
    vec4 textureColor = texture(sampler, ioVertexTexCoord);
    vec4 objectColor = textureColor*color;
    vec3 lightColor = getAmbientLightColor(ambientLight) + getDiffuseLightColor(diffuseLight, normal);

    outputColor = objectColor*vec4(lightColor, 1.0);
    
    // Apply fog calculation only if fog is enabled
    if(fogParams.isEnabled)
    {
          float fogCoordinate = abs(ioEyeSpacePosition.z / ioEyeSpacePosition.w);
          outputColor = mix(outputColor, vec4(fogParams.color, 1.0), getFogFactor(fogParams, fogCoordinate));
    }
}