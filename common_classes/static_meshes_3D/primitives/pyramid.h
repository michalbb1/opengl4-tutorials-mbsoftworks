#pragma once

// GLM
#include <glm/glm.hpp>

// Project
#include "../staticMesh3D.h"

namespace static_meshes_3D {

/**
 * Pyramid static mesh of unit size.
 */
class Pyramid : public StaticMesh3D
{
public:
	Pyramid(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
	void renderPoints() const override;

	static glm::vec3 vertices[12]; // Array of mesh vertices
	static glm::vec2 textureCoordinates[3]; // Array of mesh texture coordinates

private:
	void initializeData() override;
};

} // namespace static_meshes_3D