#version 440 core

#include "../common/ambientLight.frag"
#include "../common/diffuseLight.frag"
#include "../common/utility.frag"

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;
smooth in vec3 ioVertexNormal;

uniform sampler2D sampler;
uniform vec4 color;

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;

void main()
{
	vec3 normal = normalize(ioVertexNormal);
	vec4 textureColor = texture(sampler, ioVertexTexCoord);
	vec4 objectColor = textureColor*color;
	vec3 lightColor = sumColors(getAmbientLightColor(ambientLight), getDiffuseLightColor(diffuseLight, normal));

	outputColor = objectColor*vec4(lightColor, 1.0);
}