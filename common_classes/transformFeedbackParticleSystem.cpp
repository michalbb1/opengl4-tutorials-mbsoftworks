// STL
#include <iostream>
#include <random>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "transformFeedbackParticleSystem.h"
#include "shaderManager.h"
#include "shaderProgramManager.h"

TransformFeedbackParticleSystem::TransformFeedbackParticleSystem(const int numMaxParticlesInBuffer)
	: numMaxParticlesInBuffer_(numMaxParticlesInBuffer)
	, isInitialized_(false)
	, readBufferIndex_(0)
{
	isInitialized_ = false;
	readBufferIndex_ = 0;
}

TransformFeedbackParticleSystem::~TransformFeedbackParticleSystem()
{
	releaseParticleSystem();
}

bool TransformFeedbackParticleSystem::initialize()
{
	if (isInitialized_) {
		return true;
	}

	addVaryingInt("outType", false);
	addVaryingVec3("outPosition");
	if (!initializeShadersAndVaryings()) {
		return false;
	}

	generateBuffers();
	isInitialized_ = true;

	return true;
}

void TransformFeedbackParticleSystem::updateParticles(const float deltaTime)
{
	if (!isInitialized_) {
		return;
	}

	prepareUpdateParticles(deltaTime);

	int writeBufferIndex = 1 - readBufferIndex_;

	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackID_);

	glBindVertexArray(updateVAOs_[readBufferIndex_]);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particlesVBOs_[writeBufferIndex]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, numParticlesQueryID_);
	
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, numberOfParticles_);
	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(numParticlesQueryID_, GL_QUERY_RESULT, &numberOfParticles_);

	readBufferIndex_ = writeBufferIndex;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);
}

void TransformFeedbackParticleSystem::calculateBillboardingVectors(const glm::vec3 &cameraViewVector, const glm::vec3& cameraUpVector)
{
	billboardHorizontalVector_ = glm::normalize(glm::cross(cameraViewVector, cameraUpVector));
	billboardVerticalVector_ = glm::normalize(glm::cross(cameraViewVector, -billboardHorizontalVector_));
}

void TransformFeedbackParticleSystem::renderParticles()
{
	if (!isInitialized_) {
		return;
	}

	prepareRenderParticles();

	glBindVertexArray(renderVAOs_[readBufferIndex_]);
	glDrawArrays(GL_POINTS, 1, numberOfParticles_ - 1);
}

void TransformFeedbackParticleSystem::addVaryingInt(const std::string& varyingName, bool neededForRendering)
{
	varyings_.push_back(Varying{ varyingName, GL_INT, 1, neededForRendering });
}

void TransformFeedbackParticleSystem::addVaryingFloat(const std::string& varyingName, bool neededForRendering)
{
	varyings_.push_back(Varying{ varyingName, GL_FLOAT, 1, neededForRendering });
}

void TransformFeedbackParticleSystem::addVaryingVec3(const std::string& varyingName, bool neededForRendering)
{
	varyings_.push_back(Varying{ varyingName, GL_FLOAT, 3, neededForRendering });
}

glm::vec3 TransformFeedbackParticleSystem::generateRandomNumberGeneratorSeed()
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_real_distribution<float> seedDistribution(-10.0f, 10.0f);

	return glm::vec3(seedDistribution(generator), seedDistribution(generator), seedDistribution(generator));
}

int TransformFeedbackParticleSystem::getNumMaxParticlesInBuffer() const
{
	return numMaxParticlesInBuffer_;
}

int TransformFeedbackParticleSystem::getNumParticles() const
{
	return numberOfParticles_;
}

void TransformFeedbackParticleSystem::releaseParticleSystem()
{
	if (!isInitialized_) {
		return;
	}

	glDeleteVertexArrays(2, renderVAOs_);
	glDeleteVertexArrays(2, updateVAOs_);
	glDeleteBuffers(2, particlesVBOs_);
    std::cout << "Deleted VBOs for particle system with IDs [" << particlesVBOs_[0] << ", " << particlesVBOs_[1] << "]" << std::endl;

	glDeleteTransformFeedbacks(1, &transformFeedbackID_);
	glDeleteQueries(1, &numParticlesQueryID_);

	isInitialized_ = false;
}

GLsizei TransformFeedbackParticleSystem::Varying::getByteSize() const
{
	if (glType == GL_INT) {
		return count * sizeof(int);
	}
	else if (glType == GL_FLOAT) {
		return count * sizeof(float);
	}

	throw std::runtime_error("Unsupported GL data type for transform feedback particle system!");
}

void TransformFeedbackParticleSystem::Varying::setUpVertexAttribPointer(GLuint index, GLsizei stride, GLsizei byteOffset) const
{
	glEnableVertexAttribArray(index);
	if (glType == GL_INT) {
		glVertexAttribIPointer(index, count, glType, stride, (const GLvoid*)byteOffset);
	}
	else {
		glVertexAttribPointer(index, count, glType, GL_FALSE, stride, (const GLvoid*)byteOffset);
	}
}

std::vector<std::string> TransformFeedbackParticleSystem::getVaryingNames() const
{
	std::vector<std::string> varyingNames;

	for (const auto& varying : varyings_) {
		varyingNames.push_back(varying.name);
	}

	return varyingNames;
}

void TransformFeedbackParticleSystem::generateBuffers()
{
	glGenTransformFeedbacks(1, &transformFeedbackID_);
	glGenQueries(1, &numParticlesQueryID_);
	
	const auto particleByteSize = calculateParticleByteSize();
	GLsizeiptr bufferSize = particleByteSize * numMaxParticlesInBuffer_;
	const int32_t initialGeneratorType = PARTICLE_TYPE_GENERATOR;

	glGenBuffers(2, particlesVBOs_);
	for (auto i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
		if (i == 0) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t), &initialGeneratorType);
		}
	}

	std::cout << "Created VBOs for particle system with IDs [" << particlesVBOs_[0] << ", " << particlesVBOs_[1]
	<< "] and size " << bufferSize << std::endl;

	// Set up VAOs for updating of particles
	glGenVertexArrays(2, updateVAOs_);
	for (auto i = 0; i < 2; i++)
	{
		glBindVertexArray(updateVAOs_[i]);
		glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);

		GLsizei byteOffset = 0;
		for (auto j = 0; j < int(varyings_.size()); j++)
		{
			varyings_[j].setUpVertexAttribPointer(j, particleByteSize, byteOffset);
			byteOffset += varyings_[j].getByteSize();
		}
	}

	// Set up VAOs for rendering of particles
	glGenVertexArrays(2, renderVAOs_);
	for (auto i = 0; i < 2; i++)
	{
		glBindVertexArray(renderVAOs_[i]);
		glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);

		GLsizei byteOffset = 0;
		for (auto j = 0; j < int(varyings_.size()); j++)
		{
			if (varyings_[j].neededForRendering) {
				varyings_[j].setUpVertexAttribPointer(j, particleByteSize, byteOffset);
			}

			byteOffset += varyings_[j].getByteSize();
		}
	}

	readBufferIndex_ = 0;
	numberOfParticles_ = 1;
}

GLsizei TransformFeedbackParticleSystem::calculateParticleByteSize() const
{
	GLsizei particleByteSize = 0;
	for (const auto& varying : varyings_) {
		particleByteSize += varying.getByteSize();
	}

	return particleByteSize;
}
