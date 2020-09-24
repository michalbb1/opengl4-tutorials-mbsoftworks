#include <iostream>
#include <memory>
#include <deque>
#include <random>

#include <glm/gtc/matrix_transform.hpp>

#include "../common_classes/OpenGLWindow.h"
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

#include "HUD024.h"

FlyingCamera camera(glm::vec3(0.0f, 25.0f, -60.0f), glm::vec3(0.0f, 25.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 125.0f);

std::unique_ptr<static_meshes_3D::PlainGround> plainGround;
std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::Sphere> sphere;
std::unique_ptr<static_meshes_3D::AssimpModel> barn;
std::unique_ptr<HUD024> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.1f, 0.1f, 0.1f));
shader_structs::Material shinyMaterial(1.0f, 32.0f);

constexpr int MAX_POINT_LIGHTS = 20;

class PointLightExtended : public shader_structs::PointLight
{
public:
    const double MAX_TRAVEL_DISTANCE{ 200.0 };
    glm::vec3 direction;

    PointLightExtended(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float ambientFactor,
        const float constantAttenuation, const float linearAttenuation, const float exponentialAttenuation,
        const bool isOn = true)
        : PointLight(position, color, ambientFactor, constantAttenuation, linearAttenuation, exponentialAttenuation, isOn)
        , direction(direction) {}

    void update(const float& speedMultiplier)
    {
        const auto newPosition = position + direction * speedMultiplier;
        position += direction * speedMultiplier;
        if (newPosition.x >= MAX_TRAVEL_DISTANCE || newPosition.x <= -MAX_TRAVEL_DISTANCE
            || newPosition.z >= MAX_TRAVEL_DISTANCE || newPosition.z <= -MAX_TRAVEL_DISTANCE)
        {
            direction *= -1.0f;
        }
        else
        {
            position = newPosition;
        }
    }
};

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

std::deque<PointLightExtended> pointLights;

float rotationAngle; // Rotation angle used to animate tori

void OpenGLWindow::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut024_main", "data/shaders/tut024-ubos/shader.vert");
		sm.loadFragmentShader("tut024_main", "data/shaders/tut024-ubos/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadFragmentShader(ShaderKeys::pointLight(), "data/shaders/lighting/pointLight.frag");
		sm.loadFragmentShader(ShaderKeys::utility(), "data/shaders/common/utility.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut024_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut024_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
		 
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsundown1", "jpg");
		hud = std::make_unique<HUD024>(*this);
		
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

    pointLights.push_back(PointLightExtended(glm::vec3(-60.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.3f, 0.007f, 0.00008f));
    pointLights.push_back(PointLightExtended(glm::vec3(60.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.3f, 0.007f, 0.00008f));

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWindow::renderScene()
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
    mainProgram["numPointLights"] = 0;
	skybox->render(camera.getEye(), mainProgram);

    // Now setup all lighting properties
    int pointLightIndex = 0;
    for (const auto& pointLight : pointLights)
    {
        pointLight.setUniform(mainProgram, string_utils::formatString("pointLights[{}]", pointLightIndex++));
    }
    mainProgram["numPointLights"] = pointLightIndex;

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

    // Render all point lights
    for (auto& pointLight : pointLights)
    {
        auto pointLightModelMatrix = glm::translate(glm::mat4(1.0f), pointLight.position);
        mainProgram.setModelAndNormalMatrix(pointLightModelMatrix);
        mainProgram[ShaderConstants::color()] = glm::vec4(pointLight.color, 1.0f);
        tm.getTexture("ice").bind();
        sphere->render();
        pointLight.update(sof(20.0f));
    }

	// Render HUD
    hud->renderHUD(ambientLight, *pointLights.begin());
	
	// Update variables
	rotationAngle += sof(glm::radians(30.0f));
}

void OpenGLWindow::releaseScene()
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

void OpenGLWindow::handleInput()
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

	}

	if (keyPressed(GLFW_KEY_2))
    {

	}

    if (keyPressed(GLFW_KEY_3))
    {

    }

    if (keyPressed(GLFW_KEY_4))
    {

    }

    if (keyPressed(GLFW_KEY_5))
    {

    }

    if (keyPressed(GLFW_KEY_6))
    {

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

    static std::random_device rd;
    static std::mt19937 e2(rd());

    if (keyPressedOnce(GLFW_KEY_SPACE))
    {
        auto cameraDirection = camera.getViewPoint() - camera.getEye();
        cameraDirection.y = 0.0f;
        cameraDirection = glm::normalize(cameraDirection);
        std::uniform_real_distribution<float> dist(0.4f, 0.8f);

        if (pointLights.size() == MAX_POINT_LIGHTS) {
            pointLights.pop_front();
        }
        pointLights.push_back(PointLightExtended(camera.getEye(), cameraDirection, glm::vec3(dist(e2), dist(e2), dist(e2)), 0.0f, 0.3f, 0.007f, 0.00008f));
    }

	int posX, posY, width, height;
	glfwGetWindowPos(getWindow(), &posX, &posY);
	glfwGetWindowSize(getWindow(), &width, &height);
	camera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));
	
	camera.update([this](int keyCode) {return this->keyPressed(keyCode); },
		[this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
		[this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
		[this](float f) {return this->sof(f); });
}

void OpenGLWindow::onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
