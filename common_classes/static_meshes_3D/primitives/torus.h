#pragma once

#include "../staticMeshIndexed3D.h"

namespace static_meshes_3D {

/**
	Torus static mesh with given subdivisions and radii.
*/
class Torus : public StaticMeshIndexed3D
{
public:
	Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius,
		bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
	void renderPoints() const override;

	/** \brief  Gets main torus radius.
	*   \return Main torus radius.
	*/
	float getMainRadius() const;

	/** \brief  Gets torus tube radius.
	*   \return Torus tube radius.
	*/
	float getTubeRadius() const;

private:
	int _mainSegments; //!< Number of main subdivisons (around whole torus)
	int _tubeSegments; //!< Number of tube subdivisions
	float _mainRadius; //!< Radius of torus (distance from center of torus to the center of tube)
	float _tubeRadius; //!< Radius of tube

	int _numVertices = 0; //!< Holds the total number of generated vertices
	int _numIndices = 0; //!< Holds the number of generated indices used for rendering of the torus
	int _primitiveRestartIndex = 0; //!< Index of primitive restart

	void initializeData() override;
};

} // namespace static_meshes_3D