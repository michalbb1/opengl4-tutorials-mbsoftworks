#version 440 core

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;

uniform sampler2D sampler;
uniform vec4 color;

void main()
{
    vec4 texel = texture(sampler, ioVertexTexCoord);
    texel = vec4(texel.r, texel.r, texel.r, texel.r);
    if(texel.a == 0.0) {
        discard;
    }
    outputColor = texel*color;
}