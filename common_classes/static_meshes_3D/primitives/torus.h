#pragma once

#include "../staticMeshIndexed3D.h"

namespace static_meshes_3D {

/**
	Torus static mesh of unit size.
*/
class Torus : public StaticMeshIndexed3D
{
public:
	Torus(int stacks, int slices, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
private:
	int _stacks;
	int _slices;
	float _radius;
	float _tubeRadius;
	void initializeData() override;
};

} // namespace static_meshes_3D