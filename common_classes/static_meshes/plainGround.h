#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../vertexBufferObject.h"
#include "../texture.h"

#include "staticMesh3D.h"

namespace static_meshes {

class PlainGround : public StaticMesh3D
{
public:
	PlainGround(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	static glm::vec3 vertices[4];
	static glm::vec2 textureCoordinates[4];

	void render() const override;

protected:
	void initializeData() override;
};

} // namespace static_meshes