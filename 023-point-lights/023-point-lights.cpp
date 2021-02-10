// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "023-point-lights.h"
#include "HUD023.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/assimpModel.h"
#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/static_meshes_3D/plainGround.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"
#include "../common_classes/shader_structs/pointLight.h"

FlyingCamera camera(glm::vec3(0.0f, 25.0f, -60.0f), glm::vec3(0.0f, 25.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 125.0f);

std::unique_ptr<static_meshes_3D::PlainGround> plainGround;
std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::Sphere> sphere;
std::unique_ptr<static_meshes_3D::AssimpModel> barn;
std::unique_ptr<HUD023> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.1f, 0.1f, 0.1f));
shader_structs::Material shinyMaterial(1.0f, 32.0f);
shader_structs::PointLight pointLightA(glm::vec3(-60.0f, 20.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.3f, 0.007f, 0.00008f);
shader_structs::PointLight pointLightB(glm::vec3(60.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.3f, 0.007f, 0.00008f);
glm::vec3 dirPointLightA(0.0f, 0.0f, -1.0f);
glm::vec3 dirPointLightB(0.0f, 0.0f, 1.0f);

std::vector<glm::vec3> tripleToriPositions
{
	glm::vec3(-120.0f, 0.0f, -120.0f),
	glm::vec3(120.0f, 0.0f, -120.0f),
	glm::vec3(120.0f, 0.0f, 120.0f),
	glm::vec3(-120.0f, 0.0f, 120.0f)
};

std::vector<glm::vec3> barnPositions
{
    glm::vec3(0.0f, 0.0f, -120.0f),
    glm::vec3(-120.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 120.0f),
    glm::vec3(120.0f, 0.0f, 0.0f)
};

float rotationAngle; // Rotation angle used to animate tori

void OpenGLWindow023::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut023_main", "data/shaders/tut023-point-lights/shader.vert");
		sm.loadFragmentShader("tut023_main", "data/shaders/tut023-point-lights/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadFragmentShader(ShaderKeys::pointLight(), "data/shaders/lighting/pointLight.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut023_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut023_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
		 
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsundown1", "jpg");
		hud = std::make_unique<HUD023>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
		TextureManager::getInstance().loadTexture2D("ice", "data/textures/ice.png");
		TextureManager::getInstance().loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
        sphere = std::make_unique<static_meshes_3D::Sphere>(2.0f, 8, 8);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 8.0f, 2.0f, true, true, true);

        // Load barn, but transform the model a bit, as it's strangely rotated and a bit too big
        auto barnMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        barnMatrix = glm::scale(barnMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        barn = std::make_unique<static_meshes_3D::AssimpModel>("data/models/scheune_3ds/scheune.3ds", "", true, true, true, barnMatrix);

        plainGround = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);

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

void OpenGLWindow023::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(camera.getViewMatrix());

	// Set up some common properties in the main shader program
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
	mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	mainProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;
	
	// Render skybox first with only ambient light
    ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
	shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
    shader_structs::PointLight::none().setUniform(mainProgram, ShaderConstants::pointLightA());
    shader_structs::PointLight::none().setUniform(mainProgram, ShaderConstants::pointLightB());
	skybox->render(camera.getEye(), mainProgram);

    // Now setup all lighting properties
    pointLightA.setUniform(mainProgram, ShaderConstants::pointLightA());
    pointLightB.setUniform(mainProgram, ShaderConstants::pointLightB());

    // Render ground with only ambient light as well
    SamplerManager::getInstance().getSampler("main").bind();
    mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    tm.getTexture("grass").bind();
    plainGround->render();

	// Set up material properties for specular highlights and apply scifi metal texture
	mainProgram[ShaderConstants::eyePosition()] = camera.getEye();
	shinyMaterial.setUniform(mainProgram, ShaderConstants::material());
	TextureManager::getInstance().getTexture("scifi_metal").bind(0);

	// Render triple tori on their positions
	int i = 0;
    for (const auto& position : tripleToriPositions)
    {
        auto basicModelMatrix = glm::translate(glm::mat4(1.0f), position);
        basicModelMatrix = glm::translate(basicModelMatrix, glm::vec3(0.0f, torus->getMainRadius() + torus->getTubeRadius(), 0.0f));
        basicModelMatrix = glm::rotate(basicModelMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Render first torus with the calculated model matrix
        mainProgram.setModelAndNormalMatrix(basicModelMatrix);
        torus->render();

        // Rotate by 90 degrees around Y axis and render torus again
        // Also scale slightly, so that the tori won't overlap
        const auto secondScaleVector = i % 2 == 0 ? glm::vec3(1.01f, 1.01f, 1.01f) : glm::vec3(0.6f, 0.6f, 0.6f);
        auto secondToriModelMatrix = glm::scale(basicModelMatrix, secondScaleVector);
        secondToriModelMatrix = glm::rotate(secondToriModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mainProgram.setModelAndNormalMatrix(secondToriModelMatrix);
        torus->render();

        // Rotate by 90 degrees around X axis and render torus again
        // Also scale slightly, so that the tori won't overlap
        const auto thirdScaleVector = i % 2 == 0 ? glm::vec3(1.02f, 1.02f, 1.02f) : glm::vec3(0.36f, 0.36f, 0.36f);
        auto thirdToriModelMatrix = glm::scale(basicModelMatrix, thirdScaleVector);
        thirdToriModelMatrix = glm::rotate(thirdToriModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        mainProgram.setModelAndNormalMatrix(thirdToriModelMatrix);
        torus->render();

        i++;
    }

    // Render barns with no specular reflections
    shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
    auto barnRotationAngle = 0.0f;
    for (const auto& barnPosition : barnPositions)
    {
        auto basicModelMatrix = glm::translate(glm::mat4(1.0f), barnPosition);
        basicModelMatrix = glm::rotate(basicModelMatrix, barnRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        mainProgram.setModelAndNormalMatrix(basicModelMatrix);
        barn->render();
        barnRotationAngle += glm::pi<float>() / 2.0f;
    }

    // Render and update point light A
    auto pointLightModelMatrixA = glm::translate(glm::mat4(1.0f), pointLightA.position);
    mainProgram.setModelAndNormalMatrix(pointLightModelMatrixA);
    mainProgram["color"] = glm::vec4(pointLightA.color, 1.0f);
    tm.getTexture("ice").bind();
    sphere->render();

    pointLightA.position += sof(10.0f) * dirPointLightA;
    if (pointLightA.position.z <= -100.0f || pointLightA.position.z >= 100.0f) {
        dirPointLightA *= -1.0f;
    }

    // Render and update point light B
    auto pointLightModelMatrixB = glm::translate(glm::mat4(1.0f), pointLightB.position);
    mainProgram.setModelAndNormalMatrix(pointLightModelMatrixB);
    mainProgram["color"] = glm::vec4(pointLightB.color, 1.0f);
    tm.getTexture("ice").bind();
    sphere->render();

    pointLightB.position += sof(10.0f) * dirPointLightB;
    if (pointLightB.position.z <= -100.0f || pointLightB.position.z >= 100.0f) {
        dirPointLightB *= -1.0f;
    }

	// Render HUD
    hud->renderHUD(ambientLight, pointLightA);
}

void OpenGLWindow023::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        shinyMaterial.isEnabled = !shinyMaterial.isEnabled;
    }

    if (keyPressed(GLFW_KEY_1))
    {
        pointLightA.constantAttenuation -= sof(0.2f);
        if (pointLightA.constantAttenuation < 0.0f) {
            pointLightA.constantAttenuation = 0.0f;
        }

        pointLightB.constantAttenuation = pointLightA.constantAttenuation;
    }

    if (keyPressed(GLFW_KEY_2))
    {
        pointLightA.constantAttenuation += sof(0.2f);
        pointLightB.constantAttenuation = pointLightA.constantAttenuation;
    }

    if (keyPressed(GLFW_KEY_3))
    {
        pointLightA.linearAttenuation -= sof(0.01f);
        if (pointLightA.linearAttenuation < 0.0f) {
            pointLightA.linearAttenuation = 0.0f;
        }

        pointLightB.linearAttenuation = pointLightA.linearAttenuation;
    }

    if (keyPressed(GLFW_KEY_4))
    {
        pointLightA.linearAttenuation += sof(0.01f);
        pointLightB.linearAttenuation = pointLightA.linearAttenuation;
    }

    if (keyPressed(GLFW_KEY_5))
    {
        pointLightA.exponentialAttenuation -= sof(0.0001f);
        if (pointLightA.exponentialAttenuation < 0.0f) {
            pointLightA.exponentialAttenuation = 0.0f;
        }

        pointLightB.exponentialAttenuation = pointLightA.exponentialAttenuation;
    }

    if (keyPressed(GLFW_KEY_6))
    {
        pointLightA.exponentialAttenuation += sof(0.0001f);
        pointLightB.exponentialAttenuation = pointLightA.exponentialAttenuation;
    }

    if (keyPressed(GLFW_KEY_KP_ADD))
    {
        ambientLight.color += sof(0.2f);
        if (ambientLight.color.r > 1.0f) {
            ambientLight.color = glm::vec3(1.0f);
        }
    }

    if (keyPressed(GLFW_KEY_KP_SUBTRACT))
    {
        ambientLight.color -= sof(0.2f);
        if (ambientLight.color.r < 0.0f) {
            ambientLight.color = glm::vec3(0.0f);
        }
    }

    int posX, posY, width, height;
    glfwGetWindowPos(getWindow(), &posX, &posY);
    glfwGetWindowSize(getWindow(), &width, &height);
    camera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

    camera.update([this](int keyCode) {return this->keyPressed(keyCode); },
        [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
        [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
        [this](float f) {return this->sof(f); });

    // Update variables
    rotationAngle += sof(glm::radians(30.0f));
}

void OpenGLWindow023::releaseScene()
{
	torus.reset();
	skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

    plainGround.reset();
	hud.reset();
}
