#version 440 core

#include "diffuseLight.frag"

#include_part

struct Material
{
    bool isEnabled;
    float specularIntensity;
    float specularPower;
};

vec3 getSpecularHighlightColor(vec3 worldPosition, vec3 normal, vec3 eyePosition, Material material, DiffuseLight diffuseLight);

#definition_part

vec3 getSpecularHighlightColor(vec3 worldPosition, vec3 normal, vec3 eyePosition, Material material, DiffuseLight diffuseLight)
{
    if(!material.isEnabled) {
        return vec3(0.0);
    }

    vec3 reflectedVector = normalize(reflect(diffuseLight.direction, normal));
    vec3 worldToEyeVector = normalize(eyePosition - worldPosition);
    float specularFactor = dot(worldToEyeVector, reflectedVector);

    if (specularFactor > 0)
    {
        specularFactor = pow(specularFactor, material.specularPower);
        return diffuseLight.color * material.specularIntensity * specularFactor;
    }

    return vec3(0.0);
}