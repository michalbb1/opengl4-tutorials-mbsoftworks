// STL
#include <iostream>

// Project
#include "occlusionQuery.h"

OcclusionQuery::OcclusionQuery()
{
    glGenQueries(1, &queryID_);
    std::cout << "Created occlusion query with ID " << queryID_ << std::endl;
}

OcclusionQuery::~OcclusionQuery()
{
    glDeleteQueries(1, &queryID_);
    std::cout << "Deleted occlusion query with ID " << queryID_ << std::endl;
    queryID_ = 0;
}

void OcclusionQuery::beginQuery() const
{
    glBeginQuery(GL_SAMPLES_PASSED, queryID_);
}

void OcclusionQuery::endQuery()
{
    glEndQuery(GL_SAMPLES_PASSED);
    glGetQueryObjectiv(queryID_, GL_QUERY_RESULT, &samplesPassed_);
}

GLint OcclusionQuery::getNumSamplesPassed() const
{
    return samplesPassed_;
}

bool OcclusionQuery::anySamplesPassed() const
{
    return samplesPassed_ > 0;
}
