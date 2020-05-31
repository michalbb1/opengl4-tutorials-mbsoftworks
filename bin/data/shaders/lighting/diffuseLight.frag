#version 440 core

#include_part

struct DiffuseLight
{
    vec3 color;
    vec3 direction;
    float factor;
    bool isOn;
};

vec3 getDiffuseLightColor(DiffuseLight diffuseLight, vec3 normal);

#definition_part

vec3 getDiffuseLightColor(DiffuseLight diffuseLight, vec3 normal)
{
    if(!diffuseLight.isOn) {
        return vec3(0.0, 0.0, 0.0);
    }
    
    float finalIntensity = max(0.0, dot(normal, -diffuseLight.direction));
    finalIntensity = clamp(finalIntensity*diffuseLight.factor, 0.0, 1.0);
    return vec3(diffuseLight.color*finalIntensity);
}