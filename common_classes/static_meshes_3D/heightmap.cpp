#include "heightmap.h"

#include "../../common_classes/shaderProgramManager.h"
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace static_meshes_3D {

Heightmap::Heightmap(const GeneratorParameters& genParams, bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
{
	generateFromHeightData(generateRandomHeightData(genParams));
}

void Heightmap::generateFromHeightData(const std::vector<std::vector<float>>& heightData)
{
	if (_isInitialized) {
		deleteMesh();
	}

	_heightData = heightData;
	_rows = _heightData.size();
	_cols = _heightData[0].size();
	const auto numVertices = _rows * _cols;

	// First, prepare VAO and VBO for vertex data
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo.createVBO(numVertices*getVertexByteSize()); // Preallocate memory
	_vbo.bindVBO();

	if (hasPositions()) {
		setUpVertices();
	}

	if (hasTextureCoordinates()) {
		setUpTextureCoordinates();
	}

	if (hasNormals()) {
		if (!hasPositions()) {
			setUpVertices();
		}

		setUpNormals();
	}
	
	// Send data to GPU, they're ready now
	_vbo.uploadDataToGPU(GL_STATIC_DRAW);
	setVertexAttributesPointers(numVertices);

	// Now create a VBO with heightmap indices
	_indicesVBO.createVBO();
	_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	const auto primitiveRestartIndex = _rows * _cols;
	for (auto i = 0; i < _rows - 1; i++)
	{
		for (auto j = 0; j < _cols; j++)
			for (auto k = 0; k < 2; k++)
			{
				const auto row = i + (1 - k);
				const auto absoluteIndex = row * _cols + j;
				_indicesVBO.addData(&absoluteIndex, sizeof(int));
			}
		// Restart triangle strips
		_indicesVBO.addData(&primitiveRestartIndex, sizeof(int));
	}

	// Send indices to GPU
	_indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	// Calculate total count of indices
	_numIndices = (_rows - 1)*_cols*2 + _rows - 1;

	// Clear the data, we won't need it anymore
	_vertices.clear();
	_textureCoordinates.clear();
	_normals.clear();
	
	_isInitialized = true; // If get here, we have succeeded with generating heightmap
}

void Heightmap::render() const
{
	if (!_isInitialized) {
		return;
	}

	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
	glBindVertexArray(_vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(_rows*_cols);

	glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
}

void Heightmap::setUpVertices()
{
	_vertices = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_cols));

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _cols; j++)
		{
			const auto factorRow = float(i) / float(_rows - 1);
			const auto factorColumn = float(j) / float(_cols - 1);
			const auto& fVertexHeight = _heightData[i][j];
			_vertices[i][j] = glm::vec3(-0.5f + factorColumn, fVertexHeight, -0.5f + factorRow);
		}
		_vbo.addData(_vertices[i].data(), _cols*sizeof(glm::vec3));
	}
}

void Heightmap::setUpTextureCoordinates()
{
	_textureCoordinates = std::vector<std::vector<glm::vec2>>(_rows, std::vector<glm::vec2>(_cols));

	const auto textureStepU = 0.1f;
	const auto textureStepV = 0.1f;

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _cols; j++)
		{
			_textureCoordinates[i][j] = glm::vec2(textureStepU * j, textureStepV * j);
		}
		_vbo.addData(_textureCoordinates[i].data(), _cols * sizeof(glm::vec2));
	}
}

void Heightmap::setUpNormals()
{
	_normals = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_cols));
	std::vector< std::vector<glm::vec3> > tempNormals[2];
	for (auto i = 0; i < 2; i++) {
		tempNormals[i] = std::vector<std::vector<glm::vec3>>(_rows-1, std::vector<glm::vec3>(_cols-1));
	}

	for (auto i = 0; i < _rows - 1; i++)
	{
		for (auto j = 0; j < _cols - 1; j++)
		{
			const auto& vertexA = _vertices[i][j];
			const auto& vertexB = _vertices[i][j+1];
			const auto& vertexC = _vertices[i+1][j+1];
			const auto& vertexD = _vertices[i+1][j];

			const auto triangleNormalA = glm::cross(vertexA-vertexD, vertexB-vertexA);
			const auto triangleNormalB = glm::cross(vertexC-vertexB, vertexD-vertexC);

			tempNormals[0][i][j] = glm::normalize(triangleNormalA);
			tempNormals[1][i][j] = glm::normalize(triangleNormalB);
		}
	}

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _cols; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals

			auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i - 1][j - 1];
				}
			}

			// Look for upper-right triangles
			if (i != 0 && j != _cols - 1) {
				finalVertexNormal += tempNormals[0][i - 1][j];
			}

			// Look for bottom-right triangles
			if (i != _rows - 1 && j != _cols - 1) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i][j];
				}
			}
			// Look for bottom-left triangles
			if (i != _rows - 1 && j != 0) {
				finalVertexNormal += tempNormals[1][i][j - 1];
			}

			// Store final normal of j-th vertex in i-th row
			_normals[i][j] = glm::normalize(finalVertexNormal);
		}
		_vbo.addData(_normals[i].data(), _cols * sizeof(glm::vec3));
	}
}

std::vector<std::vector<float>> Heightmap::generateRandomHeightData(const GeneratorParameters& genParams)
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

		for (auto r = hillCenterRow - hillRadius; r < hillCenterRow + hillRadius; r++)
		{
			for (auto c = hillCenterCol - hillRadius; c < hillCenterCol + hillRadius; c++)
			{
				if (r < 0 || r >= genParams.rows || c < 0 || c >= genParams.cols) {
					continue;
				}
				const auto r2 = hillRadius * hillRadius; // r*r term
				const auto x2x1 = hillCenterCol - c; // (x2-x1) term
				const auto y2y1 = hillCenterRow - r;// (y2-y1) term
				const auto height = float(r2 - x2x1 * x2x1 - y2y1 * y2y1);
				if (height < 0.0f) {
					continue;
				}
				const auto factor = height / r2;
				heightData[r][c] += hillHeight * factor;
				if (heightData[r][c] > 1.0f) {
					heightData[r][c] = 1.0f;
				}
			}
		}
	}
	return heightData;
}

} // namespace static_meshes_3D