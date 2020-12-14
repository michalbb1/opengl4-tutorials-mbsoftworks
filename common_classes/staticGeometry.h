#pragma once

// GLM
#include <glm/glm.hpp>

namespace static_geometry
{
    // A simple cube with unit size
    extern glm::vec3 cubeVertices[36];
    extern glm::vec2 cubeTexCoords[6];
    extern glm::vec3 cubeFaceColors[6];

    // A simple pyramid with unit size
    extern glm::vec3 pyramidVertices[12];
    extern glm::vec2 pyramidTexCoords[3];
    extern glm::vec3 pyramidFaceColors[3];

    // Ground, that has greenish color
    extern glm::vec3 plainGroundVertices[4];
    extern glm::vec2 plainGroundTexCoords[4];
    extern glm::vec3 plainGroundColors[4];

    // 2D quad of unit size, anchored in lower-left point (same as texture coordinates, render with triangle strip)
    extern glm::vec2 quad2D[4];
}
