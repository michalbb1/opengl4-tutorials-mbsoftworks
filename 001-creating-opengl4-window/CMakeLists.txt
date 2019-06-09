#-------------------------------
# CPPDAQ CONFIGURATION
#-------------------------------

project(001-creating-opengl4-window) # Project's name
cmake_minimum_required(VERSION 3.1) # Specify the minimum version for CMake
set (CMAKE_CXX_STANDARD 14) # Support C++14

file(GLOB TUTORIAL_001_SOURCE_FILES *.cpp)

set(COMMON_CLASSES_SOURCE_FILES
"${COMMON_CLASSES_ROOT}/OpenGLWindow.cpp"
)

set(COMMON_CLASSES_HEADER_FILES
"${COMMON_CLASSES_ROOT}/OpenGLWindow.h"
)

# Need to add header files as well in order for source grouping to work
set(ALL_SOURCE_FILES
${TUTORIAL_001_SOURCE_FILES}
${GLAD_SOURCE_FILES}
${GLAD_HEADER_FILES}
${COMMON_CLASSES_SOURCE_FILES}
${COMMON_CLASSES_HEADER_FILES}
)

# Setup include directories of the tutorial
include_directories("${GLFW_INCLUDE_DIRS}")
include_directories("${GLAD_INCLUDE_DIRS}")
include_directories("${GLM_INCLUDE_DIRS}")

# Add directory for cpprest sdk

add_executable(001-creating-opengl4-window ${ALL_SOURCE_FILES})
target_link_libraries(001-creating-opengl4-window ${GLFW_STATIC_LIBRARIES})

set_target_properties(001-creating-opengl4-window
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXECUTABLE_OUTPUT_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXECUTABLE_OUTPUT_PATH}"
)
if(IS_64BIT_BUILD)
	set_target_properties(001-creating-opengl4-window
		PROPERTIES
		OUTPUT_NAME_DEBUG 001-creating-opengl4-window_d64
		OUTPUT_NAME_RELEASE 001-creating-opengl4-window64
	)
else()
	set_target_properties(001-creating-opengl4-window
		PROPERTIES
		OUTPUT_NAME_DEBUG 001-creating-opengl4-window_d
		OUTPUT_NAME_RELEASE 001-creating-opengl4-window
	)
endif()

#-------------------------------
# SOURCE GROUPING
#-------------------------------

source_group("Source Files\\common_classes" FILES ${COMMON_CLASSES_SOURCE_FILES})
source_group("Source Files\\glad" FILES ${GLAD_SOURCE_FILES})

source_group("Header Files\\common_classes" FILES ${COMMON_CLASSES_HEADER_FILES})
source_group("Header Files\\glad" FILES ${GLAD_HEADER_FILES})