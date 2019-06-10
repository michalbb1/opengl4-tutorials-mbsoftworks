#-------------------------------
# TUTORIAL CONFIGURATION
#-------------------------------

set(PROJECT_NAME "007-textures-pt1-single-texture")
project(${PROJECT_NAME}) # Project's name
cmake_minimum_required(VERSION 3.1) # Specify the minimum version for CMake
set (CMAKE_CXX_STANDARD 14) # Support C++14

file(GLOB TUTORIAL_007_SOURCE_FILES *.cpp)

set(COMMON_CLASSES_SOURCE_FILES
"${COMMON_CLASSES_ROOT}/flyingCamera.cpp"
"${COMMON_CLASSES_ROOT}/OpenGLWindow.cpp"
"${COMMON_CLASSES_ROOT}/sampler.cpp"
"${COMMON_CLASSES_ROOT}/shader.cpp"
"${COMMON_CLASSES_ROOT}/shaderProgram.cpp"
"${COMMON_CLASSES_ROOT}/staticGeometry.cpp"
"${COMMON_CLASSES_ROOT}/texture.cpp"
"${COMMON_CLASSES_ROOT}/uniform.cpp"
"${COMMON_CLASSES_ROOT}/vertexBufferObject.cpp"
)

set(COMMON_CLASSES_HEADER_FILES
"${COMMON_CLASSES_ROOT}/flyingCamera.h"
"${COMMON_CLASSES_ROOT}/OpenGLWindow.h"
"${COMMON_CLASSES_ROOT}/sampler.h"
"${COMMON_CLASSES_ROOT}/shader.h"
"${COMMON_CLASSES_ROOT}/shaderProgram.h"
"${COMMON_CLASSES_ROOT}/staticGeometry.h"
"${COMMON_CLASSES_ROOT}/stringUtils.h"
"${COMMON_CLASSES_ROOT}/texture.h"
"${COMMON_CLASSES_ROOT}/uniform.h"
"${COMMON_CLASSES_ROOT}/vertexBufferObject.h"
)

set(STB_HEADER_FILES
"${STB_INCLUDE_DIRS}/stb_image.h"
)

# Need to add header files as well in order for source grouping to work
set(ALL_SOURCE_FILES
${TUTORIAL_007_SOURCE_FILES}
${GLAD_SOURCE_FILES}
${GLAD_HEADER_FILES}
${COMMON_CLASSES_SOURCE_FILES}
${COMMON_CLASSES_HEADER_FILES}
${STB_HEADER_FILES}
)

# Setup include directories of the tutorial
include_directories("${GLFW_INCLUDE_DIRS}")
include_directories("${GLAD_INCLUDE_DIRS}")
include_directories("${GLM_INCLUDE_DIRS}")
include_directories("${STB_INCLUDE_DIRS}")

# Add executable and link with libraries
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXECUTABLE_OUTPUT_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXECUTABLE_OUTPUT_PATH}"
)

if(IS_64BIT_BUILD)
	set_target_properties(${PROJECT_NAME}
		PROPERTIES
		OUTPUT_NAME_DEBUG "${PROJECT_NAME}_d64"
		OUTPUT_NAME_RELEASE "${PROJECT_NAME}64"
	)
else()
	set_target_properties(${PROJECT_NAME}
		PROPERTIES
		OUTPUT_NAME_DEBUG "${PROJECT_NAME}_d"
		OUTPUT_NAME_RELEASE ${PROJECT_NAME}
	)
endif()

#-------------------------------
# SOURCE GROUPING
#-------------------------------

source_group("Source Files\\common_classes" FILES ${COMMON_CLASSES_SOURCE_FILES})
source_group("Source Files\\glad" FILES ${GLAD_SOURCE_FILES})

source_group("Header Files\\common_classes" FILES ${COMMON_CLASSES_HEADER_FILES})
source_group("Header Files\\glad" FILES ${GLAD_HEADER_FILES})
source_group("Header Files\\stb" FILES ${STB_HEADER_FILES})