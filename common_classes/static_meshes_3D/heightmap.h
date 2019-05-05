#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../vertexBufferObject.h"
#include "../texture.h"

#include "staticMeshIndexed3D.h"
#include "primitives/cube.h"
#include "primitives/pyramid.h"

namespace static_meshes_3D {

/**
	Heightmap class providing ability to generate random terrain.
*/
class Heightmap : public StaticMeshIndexed3D
{
public:
	struct GeneratorParameters
	{
		int rows;
		int cols;
		int numHills;
		int hillRadiusMin;
		int hillRadiusMax;
		float hillMinHeight;
		float hillMaxHeight;
	};

	Heightmap(const GeneratorParameters& genParams, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void generateRandomHeightmap(const GeneratorParameters& genParams);

	void render() const override;

private:
	static std::vector<std::vector<float>> generateRandomHeightData(const GeneratorParameters& genParams);

	int _numIndices;
	int _rows = 0;
	int _cols = 0;
};

} // namespace static_meshes_3D