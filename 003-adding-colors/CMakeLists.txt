#-------------------------------
# CPPDAQ CONFIGURATION
#-------------------------------

project(003-adding-colors) # Project's name
cmake_minimum_required(VERSION 3.1) # Specify the minimum version for CMake
set (CMAKE_CXX_STANDARD 14) # Support C++14

file(GLOB TUTORIAL_003_SOURCE_FILES *.cpp)

set(COMMON_CLASSES_SOURCE_FILES
"${COMMON_CLASSES_ROOT}/OpenGLWindow.cpp"
"${COMMON_CLASSES_ROOT}/shader.cpp"
"${COMMON_CLASSES_ROOT}/shaderProgram.cpp"
"${COMMON_CLASSES_ROOT}/uniform.cpp"
"${COMMON_CLASSES_ROOT}/vertexBufferObject.cpp"
)

set(COMMON_CLASSES_HEADER_FILES
"${COMMON_CLASSES_ROOT}/OpenGLWindow.h"
"${COMMON_CLASSES_ROOT}/shader.h"
"${COMMON_CLASSES_ROOT}/shaderProgram.h"
"${COMMON_CLASSES_ROOT}/stringUtils.h"
"${COMMON_CLASSES_ROOT}/uniform.h"
"${COMMON_CLASSES_ROOT}/vertexBufferObject.h"
)

# Need to add header files as well in order for source grouping to work
set(ALL_SOURCE_FILES
${TUTORIAL_003_SOURCE_FILES}
${GLAD_SOURCE_FILES}
${GLAD_HEADER_FILES}
${COMMON_CLASSES_SOURCE_FILES}
${COMMON_CLASSES_HEADER_FILES}
)

# Setup include directories of the tutorial
include_directories("${GLFW_INCLUDE_DIRS}")
include_directories("${GLAD_INCLUDE_DIRS}")
include_directories("${GLM_INCLUDE_DIRS}")

# Add executable and link with libraries
add_executable(003-adding-colors ${ALL_SOURCE_FILES})
target_link_libraries(003-adding-colors ${GLFW_STATIC_LIBRARIES})

set_target_properties(003-adding-colors
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXECUTABLE_OUTPUT_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXECUTABLE_OUTPUT_PATH}"
)
if(IS_64BIT_BUILD)
	set_target_properties(003-adding-colors
		PROPERTIES
		OUTPUT_NAME_DEBUG 003-adding-colors_d64
		OUTPUT_NAME_RELEASE 003-adding-colors64
	)
else()
	set_target_properties(003-adding-colors
		PROPERTIES
		OUTPUT_NAME_DEBUG 003-adding-colors_d
		OUTPUT_NAME_RELEASE 003-adding-colors
	)
endif()

#-------------------------------
# SOURCE GROUPING
#-------------------------------

source_group("Source Files\\common_classes" FILES ${COMMON_CLASSES_SOURCE_FILES})
source_group("Source Files\\glad" FILES ${GLAD_SOURCE_FILES})

source_group("Header Files\\common_classes" FILES ${COMMON_CLASSES_HEADER_FILES})
source_group("Header Files\\glad" FILES ${GLAD_HEADER_FILES})