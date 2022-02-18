// STL
#include <iostream>
#include <memory>
#include <deque>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project
#include "024-uniform-buffer-object.h"
#include "HUD024.h"
#include "pointLightExtended.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/uniformBufferObject.h"

#include "../common_classes/static_meshes_3D/assimpModel.h"
#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/static_meshes_3D/heightmap.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"
#include "../common_classes/shader_structs/pointLight.h"

namespace opengl4_mbsoftworks {
namespace tutorial024 {

constexpr int MAX_POINT_LIGHTS = 20;

FlyingCamera camera(glm::vec3(160.0f, 50.0f, -150.0f), glm::vec3(160.0f, 50.0f, -149.0f), glm::vec3(0.0f, 1.0f, 0.0f), 125.0f);

std::unique_ptr<static_meshes_3D::Heightmap> heightmap;
const glm::vec3 heightMapSize(400.0f, 40.0f, 400.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::Sphere> sphere;
std::unique_ptr<static_meshes_3D::AssimpModel> barn;
std::unique_ptr<HUD024> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.1f, 0.1f, 0.1f));
shader_structs::Material shinyMaterial(1.0f, 32.0f);

std::vector<glm::vec3> tripleToriPositions
{
	glm::vec3(-140.0f, 0.0f, -140.0f),
	glm::vec3(140.0f, 0.0f, -140.0f),
	glm::vec3(140.0f, 0.0f, 140.0f),
	glm::vec3(-140.0f, 0.0f, 140.0f)
};

std::vector<glm::vec3> barnPositions
{
    glm::vec3(0.0f, 0.0f, -160.0f),
    glm::vec3(-170.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 160.0f),
    glm::vec3(170.0f, 0.0f, 0.0f),
    glm::vec3(-20.0f, 40.0f, 0.0f)
};

float rotationAngle; // Rotation angle used to animate tori

std::unique_ptr<UniformBufferObject> uboMatrices; // UBO for matrices

std::deque<PointLightExtended> pointLights; // All point lights are stored here
std::unique_ptr<UniformBufferObject> uboPointLights; // UBO for point lights

void OpenGLWindow024::initializeScene()
{
	try
	{
		// Create UBO for matrices and bind it to the MATRICES_BLOCK_BINDING_POINT
        uboMatrices = std::make_unique<UniformBufferObject>();
        uboMatrices->createUBO(sizeof(glm::mat4) * 2);
        uboMatrices->bindBufferBaseToBindingPoint(UniformBlockBindingPoints::MATRICES);

		// Create UBO for point lights and bind it to the POINT_LIGHTS_BLOCK_BINDING_POINT
        uboPointLights = std::make_unique<UniformBufferObject>();
        uboPointLights->createUBO(MAX_POINT_LIGHTS * shader_structs::PointLight::getDataSizeStd140());
        uboPointLights->bindBufferBaseToBindingPoint(UniformBlockBindingPoints::POINT_LIGHTS);

		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut024_main", "data/shaders/tut024-ubos/shader.vert");
		sm.loadFragmentShader("tut024_main", "data/shaders/tut024-ubos/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadFragmentShader(ShaderKeys::pointLight(), "data/shaders/lighting/pointLight.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut024_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut024_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
        
        auto& heightmapShaderProgram = spm.createShaderProgram(static_meshes_3D::Heightmap::MULTILAYER_SHADER_PROGRAM_KEY);
        sm.loadVertexShader(static_meshes_3D::Heightmap::MULTILAYER_SHADER_PROGRAM_KEY, "data/shaders/tut024-ubos/multilayer_heightmap.vert");
        sm.loadFragmentShader(static_meshes_3D::Heightmap::MULTILAYER_SHADER_PROGRAM_KEY, "data/shaders/tut024-ubos/multilayer_heightmap.frag");

        heightmapShaderProgram.addShaderToProgram(sm.getVertexShader(static_meshes_3D::Heightmap::MULTILAYER_SHADER_PROGRAM_KEY));
        heightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(static_meshes_3D::Heightmap::MULTILAYER_SHADER_PROGRAM_KEY));

        heightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
        heightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        heightmapShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
		 
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsundown1", "jpg");
		hud = std::make_unique<HUD024>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
        TextureManager::getInstance().loadTexture2D("ice", "data/textures/ice.png");
        TextureManager::getInstance().loadTexture2D("cobblestone_mossy", "data/textures/cobblestone_mossy.png");
        TextureManager::getInstance().loadTexture2D("rocky_terrain", "data/textures/rocky_terrain.jpg");
		TextureManager::getInstance().loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
        sphere = std::make_unique<static_meshes_3D::Sphere>(2.0f, 8, 8);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 8.0f, 2.0f, true, true, true);

        // Load barn, but transform the model a bit, as it's strangely rotated and a bit too big
        auto barnMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        barnMatrix = glm::scale(barnMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        barn = std::make_unique<static_meshes_3D::AssimpModel>("data/models/scheune_3ds/scheune.3ds", "", true, true, true, barnMatrix);

        heightmap = std::make_unique<static_meshes_3D::Heightmap>("data/heightmaps/tut019.png", true, true, true);

		spm.linkAllPrograms();

        // Bind uniform blocks with binding points for main program
		mainShaderProgram.bindUniformBlockToBindingPoint("MatricesBlock", UniformBlockBindingPoints::MATRICES);
		mainShaderProgram.bindUniformBlockToBindingPoint("PointLightsBlock", UniformBlockBindingPoints::POINT_LIGHTS);
       
        // Bind uniform blocks with binding points for custom multilayer heightmap shader program
        heightmapShaderProgram.bindUniformBlockToBindingPoint("MatricesBlock", UniformBlockBindingPoints::MATRICES);
        heightmapShaderProgram.bindUniformBlockToBindingPoint("PointLightsBlock", UniformBlockBindingPoints::POINT_LIGHTS);
	}
	catch (const std::runtime_error& ex)
	{
		std::cout << "Error occured during initialization: " << ex.what() << std::endl;
		closeWindow(true);
		return;
	}

    // Create two initial point lights
    pointLights.push_back(PointLightExtended::createRandomPointLight(glm::vec3(-60.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    pointLights.push_back(PointLightExtended::createRandomPointLight(glm::vec3(60.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWindow024::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(camera.getViewMatrix());

    // Set matrices uniform buffer object data - we just set projection and view matrix here, they are
    // consistent across all shader programs
    uboMatrices->bindUBO();
    uboMatrices->setBufferData(0, glm::value_ptr(getProjectionMatrix()), sizeof(glm::mat4));
    uboMatrices->setBufferData(sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()), sizeof(glm::mat4));

    // Set point lights uniform buffer object data - in our case the poing lights are same across all shader programs
    uboPointLights->bindUBO();
    GLsizeiptr offset = 0;
    for (const auto& pointLight : pointLights)
    {
        uboPointLights->setBufferData(offset, pointLight.getDataPointer(), pointLight.getDataSizeStd140());
        offset += pointLight.getDataSizeStd140();
    }

	// Set up some common properties in the main shader program
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;
	
	// Render skybox first with only ambient light and without point lights
    // Btw it's a cool effect if you leave point lights here, looks like the sky is illuminated :)
    ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
	shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
    mainProgram[ShaderConstants::numPointLights()] = 0;
	skybox->render(camera.getEye(), mainProgram);

	// Set up material properties for specular highlights and restore point light effects
    SamplerManager::getInstance().getSampler("main").bind();
	mainProgram[ShaderConstants::eyePosition()] = camera.getEye();
	shinyMaterial.setUniform(mainProgram, ShaderConstants::material());
    mainProgram[ShaderConstants::numPointLights()] = static_cast<int>(pointLights.size());
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
    for (const auto& barnPosition : barnPositions)
    {
        auto basicModelMatrix = glm::translate(glm::mat4(1.0f), barnPosition);
        mainProgram.setModelAndNormalMatrix(basicModelMatrix);
        barn->render();
    }

    // Render all point lights
    for (auto& pointLight : pointLights)
    {
        auto pointLightModelMatrix = glm::translate(glm::mat4(1.0f), pointLight.position);
        mainProgram.setModelAndNormalMatrix(pointLightModelMatrix);
        mainProgram[ShaderConstants::color()] = glm::vec4(pointLight.color, 1.0f);
        tm.getTexture("ice").bind();
        sphere->render();
        pointLight.update(sof(20.0f), heightmap->getRenderedHeightAtPosition(heightMapSize, pointLight.position) + sphere->getRadius() + 1.0f);
    }

    // Render heightmap
    auto& heightmapShaderProgram = static_meshes_3D::Heightmap::getMultiLayerShaderProgram();
    heightmapShaderProgram.useProgram();

    // Use common properties like lighting setup
    heightmapShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ambientLight.setUniform(heightmapShaderProgram, ShaderConstants::ambientLight());
    shader_structs::DiffuseLight::none().setUniform(heightmapShaderProgram, ShaderConstants::diffuseLight());

    // Set number of point lights
    heightmapShaderProgram[ShaderConstants::numPointLights()] = static_cast<int>(pointLights.size());

    // Finally set heightmap model matrix and render it
    const auto heightmapModelMatrix = glm::scale(glm::mat4(1.0f), heightMapSize);
    heightmapShaderProgram.setModelAndNormalMatrix(heightmapModelMatrix);
    heightmap->renderMultilayered({ "cobblestone_mossy", "grass", "rocky_terrain" }, { 0.2f, 0.3f, 0.55f, 0.7f });

	// Render HUD
    hud->renderHUD(ambientLight, *pointLights.begin(), static_cast<int>(pointLights.size()));
}

void OpenGLWindow024::updateScene()
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

    auto& firstPointLight = *pointLights.begin();

    auto syncPointLightsAttenuations = [this, &firstPointLight]()
    {
        for (auto& pointLight : pointLights)
        {
            pointLight.constantAttenuation = firstPointLight.constantAttenuation;
            pointLight.linearAttenuation = firstPointLight.linearAttenuation;
            pointLight.exponentialAttenuation = firstPointLight.exponentialAttenuation;
        }
    };

    if (keyPressed(GLFW_KEY_1))
    {
        firstPointLight.constantAttenuation -= sof(0.2f);
        if (firstPointLight.constantAttenuation < 0.0f) {
            firstPointLight.constantAttenuation = 0.0f;
        }

        syncPointLightsAttenuations();
    }

    if (keyPressed(GLFW_KEY_2))
    {
        firstPointLight.constantAttenuation += sof(0.2f);
        syncPointLightsAttenuations();
    }

    if (keyPressed(GLFW_KEY_3))
    {
        firstPointLight.linearAttenuation -= sof(0.01f);
        if (firstPointLight.linearAttenuation < 0.0f) {
            firstPointLight.linearAttenuation = 0.0f;
        }

        syncPointLightsAttenuations();
    }

    if (keyPressed(GLFW_KEY_4))
    {
        firstPointLight.linearAttenuation += sof(0.01f);
        syncPointLightsAttenuations();
    }

    if (keyPressed(GLFW_KEY_5))
    {
        firstPointLight.exponentialAttenuation -= sof(0.0001f);
        if (firstPointLight.exponentialAttenuation < 0.0f) {
            firstPointLight.exponentialAttenuation = 0.0f;
        }

        syncPointLightsAttenuations();
    }

    if (keyPressed(GLFW_KEY_6))
    {
        firstPointLight.exponentialAttenuation += sof(0.0001f);
        syncPointLightsAttenuations();
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

    if (keyPressedOnce(GLFW_KEY_SPACE))
    {
        if (pointLights.size() == MAX_POINT_LIGHTS) {
            pointLights.pop_front();
        }

        pointLights.push_back(PointLightExtended::createRandomPointLight(camera.getEye(), camera.getViewPoint() - camera.getEye()));
        syncPointLightsAttenuations();
    }

    if (keyPressedOnce('R'))
    {
        while (pointLights.size() != 1) {
            pointLights.pop_front();
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

void OpenGLWindow024::releaseScene()
{
	torus.reset();
	skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

    uboMatrices.reset();
    uboPointLights.reset();

    heightmap.reset();
	hud.reset();
}

} // namespace tutorial024
} // namespace opengl4_mbsoftworks