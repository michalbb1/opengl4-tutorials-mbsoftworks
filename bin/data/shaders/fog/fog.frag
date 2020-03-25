#version 440 core

#include_part

struct FogParameters
{
    vec3 color; // Color to be used with fog, usually grayish
    float linearStart; // This is where linear fog starts (valid for linear equation only)
    float linearEnd; // This is where linear fog ends (valid for linear equation only)
    float density; // Density of the fog, used by exp and exp2 equation
    
    int equation; // Used fog equation, 3 options are valid - 0 = linear, 1 = exp, 2 = exp2
    bool isEnabled; // Flag telling if fog is enabled or not
};

float getFogFactor(FogParameters params, float fogCoordinate);

#definition_part

float getFogFactor(FogParameters params, float fogCoordinate)
{
    float result = 0.0;
    if(params.equation == 0)
    {
        float fogLength = params.linearEnd - params.linearStart;
        result = (params.linearEnd - fogCoordinate) / fogLength;
    }
    else if(params.equation == 1) {
        result = exp(-params.density * fogCoordinate);
    }
    else if(params.equation == 2) {
        result = exp(-pow(params.density * fogCoordinate, 2.0));
    }
    
    result = 1.0 - clamp(result, 0.0, 1.0);
    return result;
}