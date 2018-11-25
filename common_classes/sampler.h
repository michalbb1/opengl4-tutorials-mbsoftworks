#pragma once

#include <glad/glad.h>

enum MagnificationFilter
{
	MAG_FILTER_NEAREST, // Nearest filter for magnification
	MAG_FILTER_BILINEAR, // Bilinear filter for magnification
};

enum MinificationFilter
{
	MIN_FILTER_NEAREST, // Nearest filter for minification
	MIN_FILTER_BILINEAR, // Bilinear filter for minification
	MIN_FILTER_NEAREST_MIPMAP, // Nearest filter for minification, but on closest mipmap
	MIN_FILTER_BILINEAR_MIPMAP, // Bilinear filter for minification, but on closest mipmap
	MIN_FILTER_TRILINEAR, // Bilinear filter for minification on two closest mipmaps, then averaged
};

class Sampler
{
public:
	void create();
	void bind(int textureUnit = 0) const;
	void deleteSampler();

	void setMagnificationFilter(MagnificationFilter magnificationFilter) const;
	void setMinificationFilter(MinificationFilter minificationFilter) const;

	void setRepeat(bool repeat) const;

private:
	GLuint _samplerID = 0;
	bool _isCreated = false;

	bool createdCheck() const;
};