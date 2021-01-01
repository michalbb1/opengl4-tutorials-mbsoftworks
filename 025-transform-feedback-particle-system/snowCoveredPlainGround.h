#pragma once

// Project
#include "../common_classes/static_meshes_3D/plainGround.h"
#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/pointLight.h"
#include "../common_classes/shader_structs/fogParameters.h"

namespace static_meshes_3D {

/**
 * Plain ground static mesh covered with snow and pavement.
 */
class SnowCoveredPlainGround : public PlainGround
{
public:
    SnowCoveredPlainGround(const shader_structs::AmbientLight& ambientLight, const shader_structs::DiffuseLight& diffuseLight, const shader_structs::PointLight& firePointLight, const shader_structs::FogParameters& fogParameters,
        bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    void render() const override;

private:
    const shader_structs::AmbientLight& ambientLight_;
    const shader_structs::DiffuseLight& diffuseLight_;
    const shader_structs::PointLight& firePointLight_;
    const shader_structs::FogParameters& fogParameters_;
};

} // namespace static_meshes_3D