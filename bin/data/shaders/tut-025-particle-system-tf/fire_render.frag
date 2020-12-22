#version 440 core

#include "../fog/fog.frag"

uniform sampler2D gSampler;
uniform FogParameters fogParams;

smooth in vec2 ioTexCoord;
smooth in vec4 ioEyeSpacePosition;
flat in vec4 ioColor;

out vec4 outputColor;

void main()
{
    vec4 vTexColor = texture2D(gSampler, ioTexCoord);
    vec4 inputColorWithFog = ioColor;
    if(fogParams.isEnabled)
    {
        float fogCoordinate = abs(ioEyeSpacePosition.z / ioEyeSpacePosition.w);
        float fogFactor = getFogFactor(fogParams, fogCoordinate);
        inputColorWithFog = vec4(mix(ioColor.xyz, fogParams.color, fogFactor), ioColor.a * (1.0-fogFactor));
    }

    outputColor = vec4(vTexColor.xyz, 1.0) * inputColorWithFog;
}