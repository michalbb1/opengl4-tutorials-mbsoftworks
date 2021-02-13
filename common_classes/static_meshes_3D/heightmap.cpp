// STL
#include <iostream>
#include <random>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// STB
#include <stb_image.h>

// Project
#include "heightmap.h"

#include "../../common_classes/textureManager.h"
#include "../../common_classes/shaderManager.h"
#include "../../common_classes/shaderProgramManager.h"

namespace static_meshes_3D {

const std::string Heightmap::MULTILAYER_SHADER_PROGRAM_KEY = "multilayer_heightmap";

Heightmap::Heightmap(const HillAlgorithmParameters& params, bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
{
    createFromHeightData(generateRandomHeightData(params));
}

Heightmap::Heightmap(const std::string& fileName, bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
{
    const auto heightData = getHeightDataFromImage(fileName);
    if (heightData.size() == 0) {
        return;
    }

    createFromHeightData(heightData);
}

void Heightmap::prepareMultiLayerShaderProgram()
{
    auto& sm = ShaderManager::getInstance();
    
    if (!sm.containsFragmentShader(ShaderKeys::ambientLight()) || !sm.containsFragmentShader(ShaderKeys::diffuseLight())) {
        throw std::runtime_error("Please load fragment shaders for ambient light and diffuse light!");
    }
    
    sm.loadVertexShader(MULTILAYER_SHADER_PROGRAM_KEY, "data/shaders/heightmap/multilayer.vert");
    sm.loadFragmentShader(MULTILAYER_SHADER_PROGRAM_KEY, "data/shaders/heightmap/multilayer.frag");

    auto& spm = ShaderProgramManager::getInstance();
    auto& multiLayerHeightmapShaderProgram = spm.createShaderProgram(MULTILAYER_SHADER_PROGRAM_KEY);
    multiLayerHeightmapShaderProgram.addShaderToProgram(sm.getVertexShader(MULTILAYER_SHADER_PROGRAM_KEY));
    multiLayerHeightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(MULTILAYER_SHADER_PROGRAM_KEY));

    multiLayerHeightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
    multiLayerHeightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
}

ShaderProgram& Heightmap::getMultiLayerShaderProgram()
{
    return ShaderProgramManager::getInstance().getShaderProgram(MULTILAYER_SHADER_PROGRAM_KEY);
}

void Heightmap::createFromHeightData(const std::vector<std::vector<float>>& heightData)
{
    if (_isInitialized) {
        deleteMesh();
    }

    _heightData = heightData;
    _rows = static_cast<int>(_heightData.size());
    _columns = static_cast<int>(_heightData[0].size());
    _numVertices = _rows * _columns;

    // First, prepare VAO and VBO for vertex data
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    _vbo.createVBO(_numVertices*getVertexByteSize()); // Preallocate memory
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
    setVertexAttributesPointers(_numVertices);

    // Vertex data are in, set up the index buffer
    setUpIndexBuffer();

    // Clear the data, we won't need it anymore
    _vertices.clear();
    _textureCoordinates.clear();
    _normals.clear();
    
    // If get here, we have succeeded with generating heightmap
    _isInitialized = true;
}

void Heightmap::render() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(_primitiveRestartIndex);

    glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
    glDisable(GL_PRIMITIVE_RESTART);
}

void Heightmap::renderMultilayered(const std::vector<std::string>& textureKeys, const std::vector<float> levels) const
{
    if (!_isInitialized) {
        return;
    }

    // If there are less than 2 textures, does not even make sense to render heightmap in multilayer way
    if (textureKeys.size() < 2) {
        return;
    }

    // Number of levels defined must be correct
    if ((textureKeys.size() - 1) * 2 != levels.size()) {
        return;
    }

    // Bind chosen textures first
    const auto& tm = TextureManager::getInstance();
    auto& heightmapShaderProgram = getMultiLayerShaderProgram();
    for (auto i = 0; i < static_cast<int>(textureKeys.size()); i++)
    {
        tm.getTexture(textureKeys[i]).bind(i);
        heightmapShaderProgram[Heightmap::ShaderConstants::terrainSampler(i)] = i;
    }

    // Set uniform levels
    heightmapShaderProgram[Heightmap::ShaderConstants::numLevels()] = static_cast<int>(levels.size());
    heightmapShaderProgram[Heightmap::ShaderConstants::levels()] = levels;

    // Finally render heightmap
    render();
}

void Heightmap::renderPoints() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);

    // Render points only
    glDrawArrays(GL_POINTS, 0, _numVertices);
}

int Heightmap::getRows() const
{
    return _rows;
}

int Heightmap::getColumns() const
{
    return _columns;
}

float Heightmap::getHeight(const int row, const int column) const
{
    if (row < 0 || row >= _rows || column < 0 || column >= _columns) {
        return 0.0f;
    }

    return _heightData[row][column];
}

float Heightmap::getRenderedHeightAtPosition(const glm::vec3& renderSize, const glm::vec3& position) const
{
    const auto halfWidth = renderSize.x / 2.0f;
    const auto halfDepth = renderSize.z / 2.0f;

    const auto row = static_cast<int>(_rows * (position.z + halfDepth) / renderSize.z);
    const auto column = static_cast<int>(_columns * (position.x + halfWidth) / renderSize.x);

    return getHeight(row, column) * renderSize.y;
}

std::vector<std::vector<float>> Heightmap::generateRandomHeightData(const HillAlgorithmParameters& params)
{
    std::vector<std::vector<float>> heightData(params.rows, std::vector<float>(params.columns, 0.0f));

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> hillRadiusDistribution(params.hillRadiusMin, params.hillRadiusMax);
    std::uniform_real_distribution<float> hillHeightDistribution(params.hillMinHeight, params.hillMaxHeight);
    std::uniform_int_distribution<int> hillCenterRowIntDistribution(0, params.rows - 1);
    std::uniform_int_distribution<int> hillCenterColIntDistribution(0, params.columns - 1);

    for (int i = 0; i < params.numHills; i++)
    {
        const auto hillCenterRow = hillCenterRowIntDistribution(generator);
        const auto hillCenterCol = hillCenterColIntDistribution(generator);
        const auto hillRadius = hillRadiusDistribution(generator);
        const auto hillHeight = hillHeightDistribution(generator);

        for (auto r = hillCenterRow - hillRadius; r < hillCenterRow + hillRadius; r++)
        {
            for (auto c = hillCenterCol - hillRadius; c < hillCenterCol + hillRadius; c++)
            {
                if (r < 0 || r >= params.rows || c < 0 || c >= params.columns) {
                    continue;
                }
                const auto r2 = hillRadius * hillRadius; // r*r term
                const auto x2x1 = hillCenterCol - c; // (x2-x1) term
                const auto y2y1 = hillCenterRow - r; // (y2-y1) term
                const auto height = static_cast<float>(r2 - x2x1 * x2x1 - y2y1 * y2y1);
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

std::vector<std::vector<float>> Heightmap::getHeightDataFromImage(const std::string& fileName)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, bytesPerPixel;
    const auto imageData = stbi_load(fileName.c_str(), &width, &height, &bytesPerPixel, 0);
    if (imageData == nullptr)
    {
        // Return empty vector in case of failure
        std::cout << "Failed to load heightmap image " << fileName << "!" << std::endl;
        return std::vector<std::vector<float>>();
    }

    std::vector<std::vector<float>> result(height, std::vector<float>(width));
    auto pixelPtr = &imageData[0];
    for (auto i = 0; i < height; i++)
    {
        for (auto j = 0; j < width; j++)
        {
            result[i][j] = static_cast<float>(*pixelPtr) / 255.0f;
            pixelPtr += bytesPerPixel;
        }
    }

    stbi_image_free(imageData);
    return result;
}

void Heightmap::setUpVertices()
{
    _vertices = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_columns));

    for (auto i = 0; i < _rows; i++)
    {
        for (auto j = 0; j < _columns; j++)
        {
            const auto factorRow = static_cast<float>(i) / static_cast<float>(_rows - 1);
            const auto factorColumn = static_cast<float>(j) / static_cast<float>(_columns - 1);
            const auto& fVertexHeight = _heightData[i][j];
            _vertices[i][j] = glm::vec3(-0.5f + factorColumn, fVertexHeight, -0.5f + factorRow);
        }
        _vbo.addRawData(_vertices[i].data(), _columns*sizeof(glm::vec3));
    }
}

void Heightmap::setUpTextureCoordinates()
{
    _textureCoordinates = std::vector<std::vector<glm::vec2>>(_rows, std::vector<glm::vec2>(_columns));

    const auto textureStepU = 0.1f;
    const auto textureStepV = 0.1f;

    for (auto i = 0; i < _rows; i++)
    {
        for (auto j = 0; j < _columns; j++) {
            _textureCoordinates[i][j] = glm::vec2(textureStepU * j, textureStepV * i);
        }
        _vbo.addRawData(_textureCoordinates[i].data(), _columns * sizeof(glm::vec2));
    }
}

void Heightmap::setUpNormals()
{
    _normals = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_columns));
    std::vector< std::vector<glm::vec3> > tempNormals[2];
    for (auto i = 0; i < 2; i++) {
        tempNormals[i] = std::vector<std::vector<glm::vec3>>(_rows-1, std::vector<glm::vec3>(_columns-1));
    }

    for (auto i = 0; i < _rows - 1; i++)
    {
        for (auto j = 0; j < _columns - 1; j++)
        {
            const auto& vertexA = _vertices[i][j];
            const auto& vertexB = _vertices[i][j+1];
            const auto& vertexC = _vertices[i+1][j+1];
            const auto& vertexD = _vertices[i+1][j];

            const auto triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
            const auto triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

            tempNormals[0][i][j] = glm::normalize(triangleNormalA);
            tempNormals[1][i][j] = glm::normalize(triangleNormalB);
        }
    }

    for (auto i = 0; i < _rows; i++)
    {
        for (auto j = 0; j < _columns; j++)
        {
            // Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
            // of all adjacent triangles' normals

            const auto isFirstRow = i == 0;
            const auto isFirstColumn = j == 0;
            const auto isLastRow = i == _rows - 1;
            const auto isLastColumn = j == _columns - 1;

            auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

            // Look for triangle to the upper-left
            if (!isFirstRow && !isFirstColumn) {
                finalVertexNormal += tempNormals[0][i-1][j-1];
            }

            // Look for triangles to the upper-right
            if (!isFirstRow && !isLastColumn) {
                for (auto k = 0; k < 2; k++) {
                    finalVertexNormal += tempNormals[k][i - 1][j];
                }
            }

            // Look for triangle to the bottom-right
            if (!isLastRow && !isLastColumn) {
                finalVertexNormal += tempNormals[0][i][j];
            }

            // Look for triangles to the bottom-right
            if (!isLastRow && !isFirstColumn) {
                for (auto k = 0; k < 2; k++) {
                    finalVertexNormal += tempNormals[k][i][j-1];
                }
            }

            // Store final normal of j-th vertex in i-th row
            _normals[i][j] = glm::normalize(finalVertexNormal);
        }
        _vbo.addRawData(_normals[i].data(), _columns * sizeof(glm::vec3));
    }
}

void Heightmap::setUpIndexBuffer()
{
    // Create a VBO with heightmap indices
    _indicesVBO.createVBO();
    _indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
    _primitiveRestartIndex = _numVertices;

    for (auto i = 0; i < _rows - 1; i++)
    {
        for (auto j = 0; j < _columns; j++)
        {
            for (auto k = 0; k < 2; k++)
            {
                const auto row = i + k;
                const auto index = row * _columns + j;
                _indicesVBO.addRawData(&index, sizeof(int));
            }
        }
        // Restart triangle strips
        _indicesVBO.addRawData(&_primitiveRestartIndex, sizeof(int));
    }

    // Send indices to GPU
    _indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

    // Calculate total count of indices
    _numIndices = (_rows - 1)*_columns * 2 + _rows - 1;
}

} // namespace static_meshes_3D