#pragma once

// GLAD
#include <glad/glad.h>

/**
 * Provides convenient access to occlusion query functionality.
 */
class OcclusionQuery
{
public:
    OcclusionQuery();
    ~OcclusionQuery();

    /**
     * Begins occlusion query. Until the query is ended, samples
     * that pass the rendering pipeline are counted.
     */
    void beginQuery() const;

    /**
     * Ends occlusion query and caches the result - number
     * of samples that passed the rendering pipeline.
     */
    void endQuery();

    /**
     * Gets number of samples that have passed the rendering pipeline.
     */
    GLint getNumSamplesPassed() const;

    /**
     * Helper method that returns if any samples have passed the rendering pipeline.
     */
    bool anySamplesPassed() const;

private:
    GLuint queryID_{ 0 }; // OpenGL query object ID
    GLint samplesPassed_{ 0 }; // Number of samples passed in last query
};

