#version 440 core

layout(location = 0) out vec4 outputColor;

smooth in vec3 ioVertexColor;
smooth in vec2 ioVertexTexCoord;

uniform sampler2D gSampler;
uniform vec4 color;

void main()
{
	vec4 texColor = texture(gSampler, ioVertexTexCoord);
	outputColor = texColor*color;
}