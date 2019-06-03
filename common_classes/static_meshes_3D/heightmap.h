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

	/**
		Struct holding all parameters to generate random height data using hill generation algorithm.
	*/
	struct HillAlgorithmParameters
	{
		HillAlgorithmParameters(int rows, int columns, int numHills, int hillRadiusMin, int hillRadiusMax, float hillMinHeight, float hillMaxHeight)
		{
			this->rows = rows;
			this->columns = columns;
			this->numHills = numHills;
			this->hillRadiusMin = hillRadiusMin;
			this->hillRadiusMax = hillRadiusMax;
			this->hillMinHeight = hillMinHeight;
			this->hillMaxHeight = hillMaxHeight;
		}

		int rows; //!< Number of desired heightmap rows
		int columns; //!< Number of desired heightmap columns
		int numHills; //!< Number of generated hills
		int hillRadiusMin; //!< Minimal radius of generated hill (in terms of number of heightmap rows / columns)
		int hillRadiusMax; //!< Maximal radius of generated hill (in terms of number of heightmap rows / columns)
		float hillMinHeight; //!< Minimal height of generated hill
		float hillMaxHeight; //!< Maximal height of generated hill
	};

	Heightmap(const HillAlgorithmParameters& params, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	/** \brief  Generates heightmap from the provided height data.
	*   \param  heightData 2D float vector containing height data - each value should be between 0.0 (lowest point) and 1.0 (highest point)
	*/
	void createFromHeightData(const std::vector<std::vector<float>>& heightData);

	/** \brief Renders heightmap. */
	void render() const override;

	/** \brief  Renders heightmap as points only. */
	void renderPoints() const override;

	/** \brief  Gets number of heightmap rows.
	*   \return Number of heightmap rows.
	*/
	int getRows() const;

	/** \brief  Gets number of heightmap columns.
	*   \return Number of heightmap columns.
	*/
	int getColumns() const;

	/** \brief  Gets height value on specified row and column position.
	*   \param  row Row to get height at
	*   \param  column Column to get height at
	*   \return Height at the specified point or 0.0, if parameters are out of bounds
	*/
	float getHeight(const int row, const int column) const;

	/** \brief  Generates random height data using hill algorithm.
	*   \param  params Parameters for hill algorithm generator
	*   \return Generated height data in a 2D float vector with random values from 0.0 to 1.0.
	*/
	static std::vector<std::vector<float>> generateRandomHeightData(const HillAlgorithmParameters& params);

private:
	/** \brief Sets up heightmap vertices. */
	void setUpVertices();

	/** \brief Sets up heightmap texture coordinates. */
	void setUpTextureCoordinates();

	/** \brief Sets up heightmap normals. */
	void setUpNormals();

	/** \brief Sets up index buffer. */
	void setUpIndexBuffer();

	std::vector<std::vector<float>> _heightData;  //!< Height data representing the current heightmap
	std::vector<std::vector<glm::vec3>> _vertices; //!< Vertices data heightmap is generated with (only valid during creation phase)
	std::vector<std::vector<glm::vec2>> _textureCoordinates; //!< Texture coordinates data heightmap is generated with (only valid during creation phase)
	std::vector<std::vector<glm::vec3>> _normals; //!< Normals data heightmap is generated with (only valid during creation phase)
	int _rows = 0; //!< Number of heightmap rows
	int _columns = 0; //!< Number of heightmap columns
};

} // namespace static_meshes_3D