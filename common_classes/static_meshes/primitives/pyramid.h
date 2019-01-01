#pragma once

#include <glad/glad.h>

#include "../../vertexBufferObject.h"
#include "../staticMesh3D.h"

#include <glm/glm.hpp>

namespace static_meshes {

class Pyramid : public StaticMesh3D
{
public:
	Pyramid(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;

	static glm::vec3 vertices[12];
	static glm::vec2 textureCoordinates[3];

private:
	void initializeData() override;
};

} // namespace static_meshes