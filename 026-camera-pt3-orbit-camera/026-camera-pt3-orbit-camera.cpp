// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project
#include "026-camera-pt3-orbit-camera.h"
#include "HUD026.h"

#include "../common_classes/orbitCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/primitives/cylinder.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

namespace opengl4_mbsoftworks {
namespace tutorial026 {

OrbitCamera orbitCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), 15.0f, 3.0f, glm::pi<float>() * 0.5f, 0.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Cube> cube;
std::unique_ptr<static_meshes_3D::Cylinder> cylinder;
std::unique_ptr<static_meshes_3D::Sphere> sphere;
std::unique_ptr<static_meshes_3D::Torus> torus;

std::unique_ptr<HUD026> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.25f, 0.25f, 0.25f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.75f);

shader_structs::Material shinyMaterial(1.0f, 32.0f);
shader_structs::Material dimMaterial(0.1f, 64.0f);
bool isWireframeModeOn = false;

bool rotatingCamera = false;
bool movingCamera = false;
double prevMousePosX, prevMousePosY;
double curMousePosX, curMousePosY;

void OpenGLWindow026::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

        sm.loadVertexShader("tut026_main", "data/shaders/tut021-specular-highlight/shader.vert");
        sm.loadFragmentShader("tut026_main", "data/shaders/tut021-specular-highlight/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
        sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut026_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut026_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
		
        skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajlands1", "jpg");
		hud = std::make_unique<HUD026>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("cobblestone_mossy", "data/textures/cobblestone_mossy.png");
        TextureManager::getInstance().loadTexture2D("crate", "data/textures/crate.png");
        TextureManager::getInstance().loadTexture2D("white_marble", "data/textures/white_marble.jpg");
		TextureManager::getInstance().loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
        cube = std::make_unique<static_meshes_3D::Cube>(true, true, true);
        cylinder = std::make_unique<static_meshes_3D::Cylinder>(2.0f, 20, 4.0f, true, true, true);
        sphere = std::make_unique<static_meshes_3D::Sphere>(2.0f, 20, 20);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 1.5f, 0.75f, true, true, true);

		spm.linkAllPrograms();
	}
	catch (const std::runtime_error& ex)
	{
		std::cout << "Error occured during initialization: " << ex.what() << std::endl;
		closeWindow(true);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWindow026::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(orbitCamera.getViewMatrix());

	// Set up some common properties in the main shader program
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
    mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
    mainProgram[ShaderConstants::viewMatrix()] = orbitCamera.getViewMatrix();
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;

    // Render skybox first with only ambient light and no material
	shader_structs::AmbientLight(glm::vec3(0.8f, 0.8f, 0.8f), true).setUniform(mainProgram, ShaderConstants::ambientLight());
	shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
    shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
	skybox->render(orbitCamera.getEye(), mainProgram);
	
	// After rendering skybox, set ambient and diffuse light from our variables
    mainProgram[ShaderConstants::eyePosition()] = orbitCamera.getEye();
    ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

    // Also set our main sampler, that doesn't clamp textures unlike skybox
    SamplerManager::getInstance().getSampler("main").bind();

    // If wireframe mode is on, we need to set polygon mode to GL_LINE
    if (isWireframeModeOn) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Render torus in the top-left corner
	TextureManager::getInstance().getTexture("scifi_metal").bind();
    shinyMaterial.setUniform(mainProgram, ShaderConstants::material());
    auto modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 3.0f, 0.0f));
    mainProgram.setModelAndNormalMatrix(modelMatrix);
    torus->render();

    // Render cube in top-right corner
    TextureManager::getInstance().getTexture("cobblestone_mossy").bind();
    dimMaterial.setUniform(mainProgram, ShaderConstants::material());
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
    mainProgram.setModelAndNormalMatrix(modelMatrix);
    cube->render();

    // Render sphere in bottom-right corner
    TextureManager::getInstance().getTexture("white_marble").bind();
    shinyMaterial.setUniform(mainProgram, ShaderConstants::material());
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -3.0f, 0.0f));
    mainProgram.setModelAndNormalMatrix(modelMatrix);
    sphere->render();

    // Render cylinder in bottom-left corner
    TextureManager::getInstance().getTexture("crate").bind();
    dimMaterial.setUniform(mainProgram, ShaderConstants::material());
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -3.0f, 0.0f));
    mainProgram.setModelAndNormalMatrix(modelMatrix);
    cylinder->render();

	// Render HUD and restore normal drawing mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    hud->renderHUD(orbitCamera, isWireframeModeOn);
}

void OpenGLWindow026::releaseScene()
{
    cube.reset();
    cylinder.reset();
    sphere.reset();
	torus.reset();
    skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
}

void OpenGLWindow026::updateScene()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        isWireframeModeOn = !isWireframeModeOn;
    }

    // Check, if user started to rotate orbit camera with left mouse button
    const auto leftMouseButtonState = glfwGetMouseButton(getWindow(), GLFW_MOUSE_BUTTON_LEFT);
    if (leftMouseButtonState == GLFW_PRESS)
    {
        if (!rotatingCamera && !movingCamera)
        {
            rotatingCamera = true;
            glfwGetCursorPos(this->getWindow(), &prevMousePosX, &prevMousePosY);
        }
    }
    else if (leftMouseButtonState == GLFW_RELEASE)
    {
        rotatingCamera = false;
    }

    // Check, if user started to move with orbit camera with middle mouse button
    const auto middleMouseButtonState = glfwGetMouseButton(getWindow(), GLFW_MOUSE_BUTTON_MIDDLE);
    if (middleMouseButtonState == GLFW_PRESS)
    {
        if (!rotatingCamera && !movingCamera)
        {
            movingCamera = true;
            glfwGetCursorPos(this->getWindow(), &prevMousePosX, &prevMousePosY);
        }
    }
    else if (middleMouseButtonState == GLFW_RELEASE)
    {
        movingCamera = false;
    }

    // Update light direction with camera everytime
    diffuseLight.direction = orbitCamera.getNormalizedViewVector();

    if (!rotatingCamera && !movingCamera) {
        return;
    }

    // Only if we're rotating or moving we should calculate delta of mouse movement
    glfwGetCursorPos(this->getWindow(), &curMousePosX, &curMousePosY);
    const auto deltaX = static_cast<float>(curMousePosX - prevMousePosX);
    const auto deltaY = static_cast<float>(curMousePosY - prevMousePosY);

    if (rotatingCamera)
    {
        orbitCamera.rotateAzimuth(deltaX * 0.01f);
        orbitCamera.rotatePolar(deltaY * 0.01f);
        prevMousePosX = curMousePosX;
        prevMousePosY = curMousePosY;
    }
    else if (movingCamera)
    {
        orbitCamera.moveHorizontal(-deltaX * 0.05f);
        orbitCamera.moveVertical(deltaY * 0.05f);
        prevMousePosX = curMousePosX;
        prevMousePosY = curMousePosY;
    }
}

void OpenGLWindow026::onMouseWheelScroll(double scrollOffsetX, double scrollOffsetY)
{
    orbitCamera.zoom(static_cast<float>(-scrollOffsetY) * 0.5f);
}

} // namespace tutorial026
} // namespace opengl4_mbsoftworks
