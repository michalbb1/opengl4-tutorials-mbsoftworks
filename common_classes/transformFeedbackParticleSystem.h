#pragma once

// GLM
#include <glm/glm.hpp>

// Project
#include "shader.h"

/**
 * Base class containing common methods / components of
 * a transform feedback based particle system.
 */
class TransformFeedbackParticleSystem
{
public:
    const static int PARTICLE_TYPE_GENERATOR{ 0 }; // Represents generator particle
    const static int PARTICLE_TYPE_NORMAL{ 1 }; // Represents normal (non-generator) particle
    
    /**
     * Creates and initializes transform feedback particle system instance.
     * 
     * @param numMaxParticlesInBuffer  Maximal number of particles to be stored in the buffer
     */
    TransformFeedbackParticleSystem(const int numMaxParticlesInBuffer);

    ~TransformFeedbackParticleSystem();

    /**
     * Initializes particle system. This function must be called after constructing the object
     * (it's deliberately not called in constructor to prevent virtual method invocation there).
     *
     * @return True, if initialization was successful or false otherwise.
     */
    bool initialize();

    /**
     * Renders all particles from the current read buffer that were
     * recorded during transform feedback.
     */
    void renderParticles();

    /**
     * Updates all particles in the current read buffer using render
     * process with transform feedback and records the updated particles
     * into the current write buffer.
     * 
     * @param deltaTime  Time passed since last frame (in seconds)
     */
    void updateParticles(const float deltaTime);
    
    /**
     * Calculates and caches vectors, that can be used for rendering
     * billboarded particles (so it seems that particles always face you).
     * 
     * @param cameraViewVector  Normalized view vector of camera
     * @param cameraUpVector    Normalized up vector of camera
     */
    void calculateBillboardingVectors(const glm::vec3& cameraViewVector, const glm::vec3& cameraUpVector);

    /**
     * Gets maximal number of particles that can be stored in the buffer.
     * This number is chosen when constructing the particle system.
     */
    int getNumMaxParticlesInBuffer() const;

    /**
     * Gets number of particles currently stored in the buffer.
     */
    int getNumParticles() const;

    /**
     * Releases all resources used by the particle system.
     */
    virtual void releaseParticleSystem();

protected:
    /**
     * Abstract method, purpose of which is to load all shaders and shader
     * programs used by the particle system and also to set up which
     * variables should be recorded.
     */
    virtual bool initializeShadersAndRecordedVariables() = 0;

    /**
     * Abstract method that is called before updating particles.
     * Its responsibility is to use the appropriate shader program
     * for updating particles, set it up correctly and generally to set up
     * anything that's needed for updating of particles.
     * 
     * @param deltaTime  Time passed since last frame (in seconds)
     */
    virtual void prepareUpdateParticles(float deltaTime) = 0;
    
    /**
     * Abstract method that is called before rendering particles.
     * Its responsibility is to use the appropriate shader program
     * for rendering particles, set it up correctly and generally to set up
     * anything that's needed for rendering of particles.
     */
    virtual void prepareRenderParticles() = 0;

    /**
     * Adds integer output variable that will be recorded during transform feedback.
     * 
     * @param name                Name of the output variable to be recorded
     * @param neededForRendering  True if this variable is needed for rendering of particles or false otherwise
     */
    void addRecordedInt(const std::string& name, bool neededForRendering = true);

    /**
     * Adds float output variable that will be recorded during transform feedback.
     * 
     * @param name                Name of the output variable to be recorded
     * @param neededForRendering  True if this variable is needed for rendering of particles or false otherwise
     */
    void addRecordedFloat(const std::string& name, bool neededForRendering = true);
 
    /**
     * Adds vec3 output variable that will be recorded during transform feedback.
     * 
     * @param name                Name of the output variable to be recorded
     * @param neededForRendering  True if this variable is needed for rendering of particles or false otherwise
     */
    void addRecordedVec3(const std::string& name, bool neededForRendering = true);

    /**
     * Generates random number generator seed. This seed is a vec3 with random
     * float values that can be passed to the shaders for random number generation.
     */
    static glm::vec3 generateRandomNumberGeneratorSeed();

    glm::vec3 billboardHorizontalVector_; // Vector for rendering billboarded particles (horizontal part)
    glm::vec3 billboardVerticalVector_; // Vector for rendering billboarded particles (vertical part)
    
    /**
     * Gets vector of names of all recorded variables.
     * This is needed for shader program before its linkage.
     */
    std::vector<std::string> getRecordedVariablesNames() const;

    /**
     * Calculates byte size of particle depending on the recorded
     * variables and their types.
     * 
     * @return Size of one particle (in bytes).
     */
    GLsizei calculateParticleByteSize() const;

private:
    /**
     * Helper struct that keeps track and holds information about
     * recorded variables during transform feedback.
     */
    struct RecordedVariable
    {
        std::string name; // Name of the recorded veriable
        GLenum glType; // Type (GL_INT, GL_FLOAT etc.)
        GLsizei count; // Count of elements (e.g. vec3 is 3 x GL_FLOAT)
        bool neededForRendering; // True if this variable is needed for rendering as well (e.g. velocity of particle is not needed)

        /**
         * Gets byte size of the recorded variable.
         */
        GLsizei getByteSize() const;

        /**
         * Enables and sets up vertex attribute pointer of this recorded variable.
         *
         * @param index       Index of vertex attribute
         * @param stride      Stride between two vertices
         * @param byteOffset  Byte offset where this variable in the buffer begins
         */
        void enableAndSetupVertexAttribPointer(GLuint index, GLsizei stride, GLsizeiptr byteOffset) const;
    };

    std::vector<RecordedVariable> recordedVariables_; // List of recorded variables

    bool isInitialized_{ false }; // Flag telling if the particle system has been initialized
    
    GLuint transformFeedbackID_; // ID of transform feedback (assigned by OpenGL)
    GLuint numParticlesQueryID_; // ID of query (assigned by OpenGL) to ask about number of recorded primitives (particles in our case)

    GLuint particlesVBOs_[2]; // IDs of VBOs for particles, one is used as input and other one is used for output
    GLuint updateVAOs_[2]; // VAOs for updating particles for both VBOs
    GLuint renderVAOs_[2]; // VAOs for rendering particles for both VBOs

    int numMaxParticlesInBuffer_; // Holds maximal number of particles stored in the VBOs
    int numberOfParticles_{ 0 }; // Holds current number of particles stored

    int readBufferIndex_{ 0 }; // Current index of read buffer (write buffer is then 1-readBufferIndex_)

    /**
     * Generates all OpenGL objects required for the transform feedback particle system.
     * This includes particles VBOs, all VAOs, transform feedback and query.
     */
    void generateOpenGLObjects();
};