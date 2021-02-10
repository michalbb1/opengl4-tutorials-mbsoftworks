#version 440 core

precision highp float;

#include "../lighting/ambientLight.frag"
#include "../lighting/diffuseLight.frag"

layout(location = 0) out vec4 outputColor;

smooth in vec2 ioVertexTexCoord;
smooth in vec3 ioVertexNormal;
smooth in float ioHeight;

uniform vec4 color;

uniform AmbientLight ambientLight;
uniform DiffuseLight diffuseLight;

uniform sampler2D terrainSampler[16];
uniform float levels[32];
uniform int numLevels;

void main()
{
    vec3 normal = normalize(ioVertexNormal);
    vec4 textureColor = vec4(0.0);
    bool isTextureColorSet = false;
    for(int i = 0; i < numLevels && !isTextureColorSet; i++)
    {
        if(ioHeight > levels[i]) {
            continue; // Not yet in between the right level
        }

        // This is the index of current sampler that we're using (texture in current layer)
        int currentSamplerIndex = i / 2;

        if(i % 2 == 0) {
            // Here there is nothing special, just take the color of current texture
            textureColor = texture(terrainSampler[currentSamplerIndex], ioVertexTexCoord);
        }
        else
        {
            // First, get color of the current texture and next texture
            int nextSamplerIndex = currentSamplerIndex+1;
            vec4 textureColorCurrent = texture(terrainSampler[currentSamplerIndex], ioVertexTexCoord);
            vec4 textureColorNext = texture(terrainSampler[nextSamplerIndex], ioVertexTexCoord);

            // Calculate factors, how much will each texture contribute
            float levelDiff = levels[i] - levels[i-1];
            float factorNext = (ioHeight - levels[i-1]) / levelDiff;
            float factorCurrent = 1.0f - factorNext;
            
            // Calculate final texture color
            textureColor = textureColorCurrent*factorCurrent + textureColorNext*factorNext;
        }

        isTextureColorSet = true;
    }

    if(!isTextureColorSet)
    {
        int lastSamplerIndex = numLevels / 2;
        textureColor = texture(terrainSampler[lastSamplerIndex], ioVertexTexCoord);
    }

    vec4 objectColor = textureColor*color;
    vec3 lightColor = getAmbientLightColor(ambientLight) + getDiffuseLightColor(diffuseLight, normal);

    outputColor = objectColor*vec4(lightColor, 1.0);
}