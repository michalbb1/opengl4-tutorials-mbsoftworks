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
{
}

TransformFeedbackParticleSystem::~TransformFeedbackParticleSystem()
{
    releaseParticleSystem();
}

bool TransformFeedbackParticleSystem::initialize()
{
    // If the initialization went successfully in the past, just return true
    if (isInitialized_) {
        return true;
    }

    // Add two default recorded attributes
    addRecordedInt("outType", false);
    addRecordedVec3("outPosition");

    // If the initialization of shaders and recorded variables fails, don't continue
    if (!initializeShadersAndRecordedVariables()) {
        return false;
    }

    // Now that all went through just fine, let's generate all necessary OpenGL objects
    generateOpenGLObjects();
    isInitialized_ = true;

    return true;
}

void TransformFeedbackParticleSystem::renderParticles()
{
    // Can't render if system is not initialized
    if (!isInitialized_) {
        return;
    }

    // Prepare rendering and then render from index 1 (at index 0, there is always generator)
    prepareRenderParticles();

    glBindVertexArray(renderVAOs_[readBufferIndex_]);
    glDrawArrays(GL_POINTS, 1, numberOfParticles_ - 1);
}

void TransformFeedbackParticleSystem::updateParticles(const float deltaTime)
{
    // Can't render if system is not initialized
    if (!isInitialized_) {
        return;
    }

    // Calculate write buffer index
    const auto writeBufferIndex = 1 - readBufferIndex_;

    // Prepare update of particles
    prepareUpdateParticles(deltaTime);

    // Bind transform feedback object, VAO for updating particles and tell OpenGL where to store recorded data
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackID_);
    glBindVertexArray(updateVAOs_[readBufferIndex_]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particlesVBOs_[writeBufferIndex]);

    // Update particles with special update shader program and also observe how many particles have been written
    // Discard rasterization - we don't want to render this, it's only about updating
    glEnable(GL_RASTERIZER_DISCARD);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, numParticlesQueryID_);
    
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, numberOfParticles_);
    glEndTransformFeedback();

    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    glGetQueryObjectiv(numParticlesQueryID_, GL_QUERY_RESULT, &numberOfParticles_);

    // Swap read / write buffers for the next frame
    readBufferIndex_ = writeBufferIndex;

    // Unbind transform feedback and restore normal rendering (don't discard anymore)
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glDisable(GL_RASTERIZER_DISCARD);
}

void TransformFeedbackParticleSystem::calculateBillboardingVectors(const glm::vec3 &cameraViewVector, const glm::vec3& cameraUpVector)
{
    billboardHorizontalVector_ = glm::normalize(glm::cross(cameraViewVector, cameraUpVector));
    billboardVerticalVector_ = glm::normalize(glm::cross(cameraViewVector, -billboardHorizontalVector_));
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
    // Don't release anything if the system was not initialized
    if (!isInitialized_) {
        return;
    }

    // Delete all allocated OpenGL objects    
    glDeleteTransformFeedbacks(1, &transformFeedbackID_);
    glDeleteQueries(1, &numParticlesQueryID_);

    glDeleteVertexArrays(2, renderVAOs_);
    glDeleteVertexArrays(2, updateVAOs_);

    std::cout << "Deleting VBOs for particle system with IDs [" << particlesVBOs_[0] << ", " << particlesVBOs_[1] << "]" << std::endl;
    glDeleteBuffers(2, particlesVBOs_);

    recordedVariables_.clear();
    isInitialized_ = false;
}

void TransformFeedbackParticleSystem::addRecordedInt(const std::string& name, bool neededForRendering)
{
    recordedVariables_.push_back(RecordedVariable{ name, GL_INT, 1, neededForRendering });
}

void TransformFeedbackParticleSystem::addRecordedFloat(const std::string& name, bool neededForRendering)
{
    recordedVariables_.push_back(RecordedVariable{ name, GL_FLOAT, 1, neededForRendering });
}

void TransformFeedbackParticleSystem::addRecordedVec3(const std::string& name, bool neededForRendering)
{
    recordedVariables_.push_back(RecordedVariable{ name, GL_FLOAT, 3, neededForRendering });
}

glm::vec3 TransformFeedbackParticleSystem::generateRandomNumberGeneratorSeed()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<float> seedDistribution(-10.0f, 10.0f);

    return glm::vec3(seedDistribution(generator), seedDistribution(generator), seedDistribution(generator));
}

std::vector<std::string> TransformFeedbackParticleSystem::getRecordedVariablesNames() const
{
    std::vector<std::string> recordedVariablesNames;

    for (const auto& recordedVariable : recordedVariables_) {
        recordedVariablesNames.push_back(recordedVariable.name);
    }

    return recordedVariablesNames;
}

GLsizei TransformFeedbackParticleSystem::calculateParticleByteSize() const
{
    GLsizei particleByteSize = 0;
    for (const auto& recordedVariable : recordedVariables_) {
        particleByteSize += recordedVariable.getByteSize();
    }

    return particleByteSize;
}

GLsizei TransformFeedbackParticleSystem::RecordedVariable::getByteSize() const
{
    if (glType == GL_INT) {
        return count * sizeof(int32_t);
    }
    else if (glType == GL_FLOAT) {
        return count * sizeof(float);
    }

    throw std::runtime_error("Unsupported GL data type for transform feedback particle system!");
}

void TransformFeedbackParticleSystem::RecordedVariable::enableAndSetupVertexAttribPointer(GLuint index, GLsizei stride, GLsizeiptr byteOffset) const
{
    glEnableVertexAttribArray(index);
    if (glType == GL_INT)
    {
        // If OpenGL type is an integer or consists of integers, the glVertexAttribIPointer must be called (notice the I letter)
        glVertexAttribIPointer(index, count, glType, stride, (const GLvoid*)byteOffset);
    }
    else
    {
        // If OpenGL type is float or consists of floats, the glVertexAttribPointer must be called
        glVertexAttribPointer(index, count, glType, GL_FALSE, stride, (const GLvoid*)byteOffset);
    }
}

void TransformFeedbackParticleSystem::generateOpenGLObjects()
{
    glGenTransformFeedbacks(1, &transformFeedbackID_);
    glGenQueries(1, &numParticlesQueryID_);
    
    // Gather some constants
    const auto particleByteSize = calculateParticleByteSize();
    GLsizeiptr bufferByteSize = particleByteSize * numMaxParticlesInBuffer_;
    const int32_t initialGeneratorType = PARTICLE_TYPE_GENERATOR;

    // Generate two VBOs - one serves as source of data (read buffer) and one is written to (write buffer)
    glGenBuffers(2, particlesVBOs_);
    for (auto i = 0; i < 2; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);
        glBufferData(GL_ARRAY_BUFFER, bufferByteSize, NULL, GL_DYNAMIC_DRAW);
        if (i == 0)
        {
            // Initialize first (read buffer) with one particle that is a generator
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t), &initialGeneratorType);
        }
    }

    std::cout << "Created VBOs for particle system with IDs [" << particlesVBOs_[0] << ", " << particlesVBOs_[1]
    << "] and size " << bufferByteSize << std::endl;

    // Set up VAOs for updating of particles
    glGenVertexArrays(2, updateVAOs_);
    for (auto i = 0; i < 2; i++)
    {
        glBindVertexArray(updateVAOs_[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);

        GLsizeiptr byteOffset = 0;
        for (size_t j = 0; j < recordedVariables_.size(); j++)
        {
            recordedVariables_[j].enableAndSetupVertexAttribPointer(j, particleByteSize, byteOffset);
            byteOffset += recordedVariables_[j].getByteSize();
        }
    }

    // Set up VAOs for rendering of particles
    glGenVertexArrays(2, renderVAOs_);
    for (auto i = 0; i < 2; i++)
    {
        glBindVertexArray(renderVAOs_[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particlesVBOs_[i]);

        GLsizeiptr byteOffset = 0;
        for (size_t j = 0; j < recordedVariables_.size(); j++)
        {
            // Enable vertex attribute only if it's needed for rendering
            // Some properties (like type of particle or velocity might not be needed)
            if (recordedVariables_[j].neededForRendering) {
                recordedVariables_[j].enableAndSetupVertexAttribPointer(j, particleByteSize, byteOffset);
            }

            byteOffset += recordedVariables_[j].getByteSize();
        }
    }

    // Initialize read buffer index and initial number of particles
    readBufferIndex_ = 0;
    numberOfParticles_ = 1;
}
