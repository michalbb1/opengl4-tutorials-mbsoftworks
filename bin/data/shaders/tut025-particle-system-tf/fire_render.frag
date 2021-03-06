#version 440 core

#include "../fog/fog.frag"

smooth in vec2 ioTexCoord;
smooth in vec4 ioEyeSpacePosition;
flat in vec4 ioColor;

uniform sampler2D sampler;
uniform FogParameters fogParams;

out vec4 outputColor;

void main()
{
    vec4 textureColor = texture(sampler, ioTexCoord);
    vec4 inputColor = ioColor;

    // If fog is enabled, mix input color with fog
    if(fogParams.isEnabled)
    {
        float fogCoordinate = abs(ioEyeSpacePosition.z / ioEyeSpacePosition.w);
        float fogFactor = getFogFactor(fogParams, fogCoordinate);
        inputColor = vec4(mix(ioColor.xyz, fogParams.color, fogFactor), ioColor.a * (1.0 - fogFactor));
    }

    outputColor = vec4(textureColor.xyz, 1.0) * inputColor;
}