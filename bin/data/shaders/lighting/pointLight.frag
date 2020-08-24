#version 440 core

#include_part

struct PointLight
{
	vec3 position;
	vec3 color;
	
	float ambientFactor;

	float constantAttenuation;
	float linearAttenuation;
	float exponentialAttenuation;
	
	bool isOn;
};

vec3 getPointLightColor(const PointLight pointLight, const vec3 worldPosition, const vec3 normal);

#definition_part

vec3 getPointLightColor(const PointLight pointLight, const vec3 worldPosition, const vec3 normal)
{
	if(!pointLight.isOn) {
		return vec3(0.0);
	}
	
	vec3 positionToLightVector = worldPosition - pointLight.position;
	float distance = length(positionToLightVector);
	positionToLightVector = normalize(positionToLightVector);
	
	float diffuseFactor = max(0.0, dot(normal, -positionToLightVector));	
	float totalAttenuation = pointLight.constantAttenuation
		+ pointLight.linearAttenuation * distance
		+ pointLight.exponentialAttenuation * pow(distance, 2.0);

	return pointLight.color * (pointLight.ambientFactor + diffuseFactor) / totalAttenuation;
}