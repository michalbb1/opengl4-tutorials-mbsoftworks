#pragma once

// STL
#include <string>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>

// Project
#include "../shaderProgram.h"
#include "../vertexBufferObject.h"

#include "staticMeshIndexed3D.h"

namespace static_meshes_3D {

/**
 * Heightmap class providing ability to generate random terrain.
 */
class Heightmap : public StaticMeshIndexed3D
{
public:
    static const std::string MULTILAYER_SHADER_PROGRAM_KEY; // Holds a key for multilayer heightmap shader program (used as shaders key too)

    struct ShaderConstants
    {
        DEFINE_SHADER_CONSTANT_INDEX(terrainSampler, "terrainSampler")
        DEFINE_SHADER_CONSTANT_INDEX(levels, "levels")
        DEFINE_SHADER_CONSTANT(numLevels, "numLevels")
    };

    /**
     * Struct holding all parameters to generate random height data using hill generation algorithm.
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

        int rows; // Number of desired heightmap rows
        int columns; // Number of desired heightmap columns
        int numHills; // Number of generated hills
        int hillRadiusMin; // Minimal radius of generated hill (in terms of number of heightmap rows / columns)
        int hillRadiusMax; // Maximal radius of generated hill (in terms of number of heightmap rows / columns)
        float hillMinHeight; // Minimal height of generated hill
        float hillMaxHeight; // Maximal height of generated hill
    };

    Heightmap(const HillAlgorithmParameters& params, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);
    Heightmap(const std::string& fileName, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    static void prepareMultiLayerShaderProgram();
    static ShaderProgram& getMultiLayerShaderProgram();

    /**
     * Generates heightmap from the provided height data.
     *
     * @param heightData  2D float vector containing height data - each value should be between 0.0 (lowest point) and 1.0 (highest point)
     */
    void createFromHeightData(const std::vector<std::vector<float>>& heightData);

    /** \brief Renders heightmap. */
    void render() const override;

    /**
     * Renders heightmap with multiple layers.
     *
     * @param textureKeys  Contains which textures should be used (ordered from bottom-most to top-most layer)
     * @param levels       Contains where within the heightmap should layer transitions start / stop
     */
    void renderMultilayered(const std::vector<std::string>& textureKeys, const std::vector<float> levels) const;

    /**
     * Renders heightmap as points only.
     */
    void renderPoints() const override;

    /**
     * Gets number of heightmap rows.
     */
    int getRows() const;

    /**
     * Gets number of heightmap columns.
     */
    int getColumns() const;

    /**
     * Gets height value on specified row and column position.
     *
     * @param row     Row to get height at
     * @param column  Column to get height at
     *
     * @return Height at the specified point or 0.0, if parameters are out of bounds.
     */
    float getHeight(const int row, const int column) const;

    /**
     * Gets rendered height at specified row and column.
     *
     * @param row     Row to get rendered height at
     * @param column  Column to get rendered height at
     *
     * @return Height at the specified point or 0.0, if parameters are out of bounds.
     */
    float getRenderedHeightAtPosition(const glm::vec3& renderSize, const glm::vec3& position) const;

    /**
     * Generates random height data using hill algorithm.
     *
     * @param params  Parameters for hill algorithm generator
     * 
     * @return Generated height data in a 2D float vector with random values from 0.0 to 1.0.
     */
    static std::vector<std::vector<float>> generateRandomHeightData(const HillAlgorithmParameters& params);

    /**
     * Gets height data from image - converts grayscale data to height data.
     *
     * @param fileName  Filename to load heightmap from
     *
     * @return Height data as a 2D float vector with values from 0.0 to 1.0.
     */
    static std::vector<std::vector<float>> getHeightDataFromImage(const std::string& fileName);

private:
    /**
     * Sets up heightmap vertices.
     */
    void setUpVertices();

    /**
     * Sets up heightmap texture coordinates.
     */
    void setUpTextureCoordinates();

    /**
     * Sets up heightmap normals.
     */
    void setUpNormals();

    /**
     * Sets up index buffer.
     */
    void setUpIndexBuffer();

    std::vector<std::vector<float>> _heightData; // Height data representing the current heightmap
    std::vector<std::vector<glm::vec3>> _vertices; // Vertices data heightmap is generated with (only valid during creation phase)
    std::vector<std::vector<glm::vec2>> _textureCoordinates; // Texture coordinates data heightmap is generated with (only valid during creation phase)
    std::vector<std::vector<glm::vec3>> _normals; // Normals data heightmap is generated with (only valid during creation phase)
    int _rows = 0; // Number of heightmap rows
    int _columns = 0; // Number of heightmap columns
};

} // namespace static_meshes_3D