#pragma once

#include <glad/glad.h>

#include "../vertexBufferObject.h"
#include "static_mesh_3D.h"

#include <glm/glm.hpp>

namespace static_meshes {

class Cube : public StaticMesh3D
{
public:
	Cube(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() override;
	void release() override;

	static glm::vec3 vertices[36];
	static glm::vec2 textureCoordinates[6];
	static glm::vec3 normals[6];
private:
	GLuint _vao;
	VertexBufferObject _vbo;

	void initializeData() override;
};

} // namespace static_meshes