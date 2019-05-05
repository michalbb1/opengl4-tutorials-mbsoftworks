#version 440 core

uniform struct
{
	mat4 projectionMatrix;
	mat4 viewMatrix;                                            
	mat4 modelMatrix;
	mat3 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec2 vTexCoord;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;
smooth out vec4 vEyeSpacePos;

uniform mat4 HeightmapScaleMatrix;

void main()
{
	vec4 inPositionScaled = HeightmapScaleMatrix* vec4(inPosition, 1.0);
	mat4 mMVP = matrices.projectionMatrix*matrices.viewMatrix*matrices.modelMatrix;

	gl_Position = mMVP*inPositionScaled;
	vEyeSpacePos = matrices.viewMatrix*matrices.modelMatrix*vec4(inPosition, 1.0);

	vTexCoord = inCoord;
	vNormal = matrices.normalMatrix*inNormal;

	vec4 vWorldPosLocal = matrices.modelMatrix*inPositionScaled;
	vWorldPos = vWorldPosLocal.xyz;
}