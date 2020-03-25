#version 440 core

#include_part

struct AmbientLight
{
    vec3 color;
    bool isOn;
};

vec3 getAmbientLightColor(AmbientLight ambientLight);

#definition_part

vec3 getAmbientLightColor(AmbientLight ambientLight)
{
    return ambientLight.isOn ? ambientLight.color : vec3(0.0, 0.0, 0.0);
}