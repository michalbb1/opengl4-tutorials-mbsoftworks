#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../vertexBufferObject.h"
#include "../texture.h"

#include "staticMesh3D.h"
#include "primitives/cube.h"
#include "primitives/pyramid.h"

namespace static_meshes {

class House : public StaticMesh3D
{
public:
	static const std::string WALLS_TEXTURE_KEY;
	static const std::string DOOR_WALL_TEXTURE_KEY;
	static const std::string WINDOWS_WALL_TEXTURE_KEY;
	static const std::string ROOF_TEXTURE_KEY;

	House(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
	void render(const glm::vec3& position, float rotationAngle) const;

	static void prepareTextures();

protected:
	void initializeData() override {}

private:
	const Texture& wallsTexture;
	const Texture& doorWallTexture;
	const Texture& windowsWallTexture;
	const Texture& roofTexture;

	Cube cube;
	Pyramid pyramid;
};

} // namespace static_meshes