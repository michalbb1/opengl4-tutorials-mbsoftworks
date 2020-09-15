#pragma once

// STL
#include <string>

// GLM
#include <glm/glm.hpp>

// Project
#include "stringUtils.h"
#include "OpenGLWindow.h"
#include "static_meshes_2D/primitives/quad.h"
#include "shaderProgram.h"
#include "sampler.h"

/**
  Base class for rendering HUD (head-up display) over scene.
*/
class HUDBase
{
public:
	static const std::string DEFAULT_FONT_KEY;
	static const std::string ORTHO_2D_PROGRAM_KEY; //!< Key for shader program for rendering in 2D
	static const std::string HUD_SAMPLER_KEY; //!< Key for sampler used for HUD objects

	HUDBase(const OpenGLWindow& window);

	//* \brief Renders HUD. */
	virtual void renderHUD() const = 0;

protected:
	const OpenGLWindow& _window; //!< OpenGL Window class instance, for which HUD is rendered

	/** \brief  Gets HUD width.
	*   \return HUD width.
	*/
	int getWidth() const;

	/** \brief  Gets HUD height.
	*   \return HUD height.
	*/
	int getHeight() const;

	/** \brief  Renders 2D textured quad at specified position with specified size. Possibility to render from right or top as well.
	*   \param  x Rendering position from left or from right edge (depending on fromRight flag)
	*   \param  y Rendering position from top or bottom edge (depending on fromTop flag)
	*   \param  renderedWidth  Width of rendered quad (in pixels)
	*   \param  renderedHeight Height of rendered quad (in pixels)
	*   \param  fromRight Flag telling, if you want to render from right side instead of from left side
	*   \param  fromTop Flag telling, if you want to render from top side instead of from bottom side
	*/
	void renderTexturedQuad2D(int x, int y, int renderedWidth, int renderedHeight, bool fromRight = false, bool fromTop = false) const;

	/** \brief  Gets shader program for 2D rendering.
	*   \return Shader program for 2D rendering.
	*/
	ShaderProgram& getOrtho2DShaderProgram() const;

	/** \brief  Gets HUD sampler.
	*   \return HUD sampler.
	*/
	const Sampler& getHUDSampler() const;

	static_meshes_2D::Quad _texturedQuad; //!< Static mesh for rendering 2D quad
};