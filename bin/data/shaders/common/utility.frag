#version 440 core

#include_part

vec3 sumColors(vec3 colorA, vec3 colorB);

#definition_part

vec3 sumColors(vec3 colorA, vec3 colorB)
{
    vec3 result = colorA+colorB;
    return clamp(result, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
}
