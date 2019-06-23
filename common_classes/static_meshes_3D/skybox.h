#pragma once

#include <string>

#include <glm/glm.hpp>

#include "staticMesh3D.h"
#include "primitives/cube.h"

namespace static_meshes_3D {

/**
	Skybox static mesh - basically cube with some additional textures
*/
class Skybox : public Cube
{
public:
	static const std::string SAMPLER_KEY; //!< Key to store skybox sampler within

	Skybox(const std::string& baseDirectory, const std::string& imageExtension, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	/** \brief Renders skybox around camera.
	*   \param cameraPosition Position to render around (usually around player, thus camera)
	*   \param shaderProgram Shader program to set matrices in
	*/
	void render(const glm::vec3& cameraPosition, ShaderProgram& shaderProgram) const;

private:
	std::string _baseDirectory; //!< Path to directory where skybox images are located
	std::string _imageExtension; //!< Image extension of images, should be consistent (png, jpg etc.)

	/** \brief  Gets filename of a specified skybox side.
	*   \param  sideBit Bit that represents the cube side (from cube.h)
	*   \return Combined directory path, side name and image extension.
	*/
	std::string getSideFileName(const int sideBit) const;

	/** \brief  Gets texture key of a specified skybox side.
	*   \param  sideBit Bit that represents the cube side (from cube.h)
	*   \return Key, within which is texture of this skybox stored.
	*/
	std::string getTextureKey(const int sideBit) const;

	/** \brief  Tries to load the texture of a specified skybox side. If suceeeds, texture is stored in manager.
	*   \param  sideBit Bit that represents the cube side (from cube.h)
	*/
	void tryLoadTexture(const int sideBit);
};

} // namespace static_meshes_3D