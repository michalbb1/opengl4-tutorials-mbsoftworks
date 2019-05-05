#include "heightmap.h"

#include "../../common_classes/shaderProgramManager.h"
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace static_meshes_3D {

Heightmap::Heightmap(const GeneratorParameters& genParams, bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
{
	generateRandomHeightmap(genParams);
}

void Heightmap::generateRandomHeightmap(const GeneratorParameters& genParams)
{
	if (_isInitialized) {
		deleteMesh();
	}

	const auto heightData = generateRandomHeightData(genParams);
	_rows = heightData.size();
	_cols = heightData[0].size();
	const auto numVertices = _rows * _cols;

	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	std::vector< std::vector< glm::vec3> > vertices(_rows, std::vector<glm::vec3>(_cols));
	std::vector< std::vector< glm::vec2> > textureCoords(_rows, std::vector<glm::vec2>(_cols));

	float fTextureU = float(_cols)*0.1f;
	float fTextureV = float(_rows)*0.1f;

	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			float fScaleC = float(j) / float(_cols - 1);
			float fScaleR = float(i) / float(_rows - 1);
			const auto& fVertexHeight = heightData[i][j];
			vertices[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			textureCoords[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
		}
	}

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	std::vector< std::vector<glm::vec3> > vNormals[2];
	for (int i = 0; i < 2; i++)
		vNormals[i] = std::vector< std::vector<glm::vec3> >(_rows - 1, std::vector<glm::vec3>(_cols - 1));

	for (int i = 0; i < _rows - 1; i++)
	{
		for (int j = 0; j < _cols - 1; j++)
		{
			glm::vec3 vTriangle0[] =
			{
				vertices[i][j],
				vertices[i + 1][j],
				vertices[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				vertices[i + 1][j + 1],
				vertices[i][j + 1],
				vertices[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

	std::vector< std::vector<glm::vec3> > vFinalNormals = std::vector< std::vector<glm::vec3> >(_rows, std::vector<glm::vec3>(_cols));

	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals

			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0)
				for (int k = 0; k < 2; k++)vFinalNormal += vNormals[k][i - 1][j - 1];
			// Look for upper-right triangles
			if (i != 0 && j != _cols - 1)vFinalNormal += vNormals[0][i - 1][j];
			// Look for bottom-right triangles
			if (i != _rows - 1 && j != _cols - 1)
				for (int k = 0; k < 2; k++)vFinalNormal += vNormals[k][i][j];
			// Look for bottom-left triangles
			if (i != _rows - 1 && j != 0)
				vFinalNormal += vNormals[1][i][j - 1];
			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
		}
	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// First, create a VBO with only vertex data
	_vbo.createVBO(_rows*_cols*(2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	_vbo.bindVBO();

	for (int i = 0; i < _rows; i++)for (int j = 0; j < _cols; j++)_vbo.addData(&vertices[i][j], sizeof(glm::vec3)); // Add vertex
	for (int i = 0; i < _rows; i++)for (int j = 0; j < _cols; j++)_vbo.addData(&textureCoords[i][j], sizeof(glm::vec2)); // Add tex. coord
	for (int i = 0; i < _rows; i++)for (int j = 0; j < _cols; j++)_vbo.addData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
	
	// Attach vertex data to this VAO
	_vbo.uploadDataToGPU(GL_STATIC_DRAW);
	setVertexAttributesPointers(numVertices);

	// Now create a VBO with heightmap indices
	_indicesVBO.createVBO();
	_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	const auto primitiveRestartIndex = _rows * _cols;
	for (int i = 0; i < _rows - 1; i++)
	{
		for (int j = 0; j < _cols; j++)
			for (int k = 0; k < 2; k++)
			{
				const auto row = i + (1 - k);
				const auto absoluteIndex = row * _cols + j;
				_indicesVBO.addData(&absoluteIndex, sizeof(int));
			}
		// Restart triangle strips
		_indicesVBO.addData(&primitiveRestartIndex, sizeof(int));
	}

	// And now attach index data to this VAO
	// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
	_indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);
	_numIndices = (_rows - 1)*_cols * 2 + _rows - 1;

	_isInitialized = true; // If get here, we succeeded with generating heightmap
}

void Heightmap::render() const
{
	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
	glBindVertexArray(_vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(_rows*_cols);

	glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
}

std::vector<std::vector<float>> Heightmap::generateRandomHeightData(const GeneratorParameters & genParams)
{
	std::vector<std::vector<float>> heightData(genParams.rows, std::vector<float>(genParams.cols, 0.0f));

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> hillRadiusDistribution(genParams.hillRadiusMin, genParams.hillRadiusMax);
	std::uniform_real_distribution<float> hillHeightDistribution(genParams.hillMinHeight, genParams.hillMaxHeight);
	std::uniform_int_distribution<int> intDistribution(0, genParams.rows - 1);

	for (int i = 0; i < genParams.numHills; i++)
	{
		const auto hillCenterRow = intDistribution(generator);
		const auto hillCenterCol = intDistribution(generator);
		const auto hillRadius = hillRadiusDistribution(generator);
		const auto hillHeight = hillHeightDistribution(generator);

		for (int r = hillCenterRow - hillRadius; r < hillCenterRow + hillRadius; r++)
		{
			for (int c = hillCenterCol - hillRadius; c < hillCenterCol + hillRadius; c++)
			{
				if (r < 0 || r >= genParams.rows || c < 0 || c >= genParams.cols) {
					continue;
				}
				const auto r2 = hillRadius * hillRadius;
				const auto x2x1 = hillCenterCol - c;
				const auto y2y1 = hillCenterRow - r;
				const auto height = float(r2 - x2x1 * x2x1 - y2y1 * y2y1);
				if (height < 0.0f) {
					continue;
				}
				float scale = height / r2;
				heightData[r][c] += hillHeight * scale;
				if (heightData[r][c] > 1.0f) {
					heightData[r][c] = 1.0f;
				}
			}
		}
	}
	return heightData;
}

} // namespace static_meshes_3D