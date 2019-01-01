#pragma once

#include <glad/glad.h>

#include "../staticMesh3D.h"

#include <glm/glm.hpp>

namespace static_meshes {

const int CUBE_FRONT_FACE = 1;
const int CUBE_BACK_FACE = 1 << 1;
const int CUBE_LEFT_FACE = 1 << 2;
const int CUBE_RIGHT_FACE = 1 << 3;
const int CUBE_TOP_FACE = 1 << 4;
const int CUBE_BOTTOM_FACE = 1 << 5;

class Cube : public StaticMesh3D
{
public:
	Cube(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
	void renderFaces(int facesBitmask) const;

	static glm::vec3 vertices[36];
	static glm::vec2 textureCoordinates[6];
	static glm::vec3 normals[6];

private:
	void initializeData() override;
};

} // namespace static_meshes