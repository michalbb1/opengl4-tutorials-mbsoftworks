#pragma once

// GLM
#include <glm/glm.hpp>

// Project
#include "shader.h"

class TransformFeedbackParticleSystem
{
public:
	const static int PARTICLE_TYPE_GENERATOR{ 0 };
	const static int PARTICLE_TYPE_NORMAL{ 1 };

	TransformFeedbackParticleSystem(const int numMaxParticlesInBuffer);
	~TransformFeedbackParticleSystem();

	bool initialize();

	void renderParticles();
	void updateParticles(const float deltaTime);
	
	void calculateBillboardingVectors(const glm::vec3& cameraViewVector, const glm::vec3& cameraUpVector);

	int getNumMaxParticlesInBuffer() const;
	int getNumParticles() const;

	virtual void releaseParticleSystem();

protected:
	virtual bool initializeShadersAndVaryings() = 0;

	virtual void prepareUpdateParticles(float deltaTime) = 0;
	virtual void prepareRenderParticles() = 0;

	void addVaryingInt(const std::string& varyingName, bool neededForRendering = true);
	void addVaryingFloat(const std::string& varyingName, bool neededForRendering = true);
	void addVaryingVec3(const std::string& varyingName, bool neededForRendering = true);

	static glm::vec3 generateRandomNumberGeneratorSeed();

	glm::vec3 billboardHorizontalVector_;
	glm::vec3 billboardVerticalVector_;

	struct Varying
	{
		std::string name;
		GLenum glType;
		GLsizei count;
		bool neededForRendering;

		GLsizei getByteSize() const;
		void setUpVertexAttribPointer(GLuint index, GLsizei stride, GLsizei byteOffset) const;
	};

	std::vector<Varying> varyings_;

	std::vector<std::string> getVaryingNames() const;
	GLsizei calculateParticleByteSize() const;
private:
	bool isInitialized_;
	
	GLuint transformFeedbackID_;
	GLuint numParticlesQueryID_;

	GLuint particlesVBOs_[2];
	GLuint updateVAOs_[2];
	GLuint renderVAOs_[2];

	int numMaxParticlesInBuffer_;
	int numberOfParticles_;

	int readBufferIndex_;

	void generateBuffers();
};