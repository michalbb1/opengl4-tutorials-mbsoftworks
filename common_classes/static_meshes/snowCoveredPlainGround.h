#pragma once

#include "plainGround.h"

namespace static_meshes {

/**
	Plain ground static mesh covered with snow and pavement.
*/
class SnowCoveredPlainGround : public PlainGround
{
public:
	SnowCoveredPlainGround(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void render() const override;
};

} // namespace static_meshes