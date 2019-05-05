#version 440 core

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

uniform sampler2D sampler;
uniform vec4 color;

uniform float fRenderHeight;
uniform float fMaxTextureU;
uniform float fMaxTextureV;

out vec4 outputColor;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	vec4 vTexColor = texture(sampler, vTexCoord);
	vec4 vMixedColor = vTexColor*color;
	outputColor = vMixedColor;
}                      