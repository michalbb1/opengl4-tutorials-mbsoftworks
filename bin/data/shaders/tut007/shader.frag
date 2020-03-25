#version 440 core

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;

uniform sampler2D sampler;
uniform vec4 color;

void main()
{
    vec4 texColor = texture(sampler, ioVertexTexCoord);
    outputColor = texColor*color;
}