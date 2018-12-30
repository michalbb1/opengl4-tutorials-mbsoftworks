#pragma once

namespace static_meshes {

class StaticMesh3D
{
public:
	StaticMesh3D(bool withPositions, bool withTextureCoordinates, bool withNormals);

	static const int POSITION_ATTRIBUTE_INDEX;
	static const int TEXTURE_COORDINATE_ATTRIBUTE_INDEX;
	static const int NORMAL_ATTRIBUTE_INDEX;

	virtual void render() = 0;
	virtual void release() = 0;

	bool hasPositions() const;
	bool hasTextureCoordinates() const;
	bool hasNormals() const;
	int getVertexByteSize() const;

protected:
	bool _hasPositions;
	bool _hasTextureCoordinates;
	bool _hasNormals;

	virtual void initializeData() = 0;
};

}; // namespace static_meshes