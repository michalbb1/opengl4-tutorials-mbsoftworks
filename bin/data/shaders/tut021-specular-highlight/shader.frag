#version 440 core

#include "../lighting/ambientLight.frag"
#include "../lighting/diffuseLight.frag"
#include "../lighting/specularHighlight.frag"

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;
smooth in vec3 ioVertexNormal;
smooth in vec4 ioWorldPosition;
smooth in vec4 ioEyeSpacePosition;

uniform sampler2D sampler;
uniform vec4 color;

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;
uniform Material material;
uniform vec3 eyePosition;

void main()
{
    vec3 normal = normalize(ioVertexNormal);
    vec4 textureColor = texture(sampler, ioVertexTexCoord);
    vec4 objectColor = textureColor*color;
    vec3 ambientColor = getAmbientLightColor(ambientLight);
    vec3 diffuseColor = getDiffuseLightColor(diffuseLight, normal);
    vec3 specularHighlightColor = getSpecularHighlightColor(ioWorldPosition.xyz, normal, eyePosition, material, diffuseLight);
    vec3 lightColor = ambientColor + diffuseColor + specularHighlightColor;

    outputColor = objectColor * vec4(lightColor, 1.0);
}