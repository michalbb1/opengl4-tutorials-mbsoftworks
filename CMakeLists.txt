#-------------------------------
# GLOBAL SETTINGS
#-------------------------------

cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)

project(opengl4-tutorials-mbsoftworks) # "Solution" name

#-------------------------------
# GLOBAL VARIABLES
#-------------------------------

# Specify, where should the tutorials be built
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configuration types" FORCE)
endif()

# Find, if it's 64 bit build
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(IS_64BIT_BUILD TRUE)
else()
	set(IS_64BIT_BUILD FALSE)
endif()

#-------------------------------
# ADDITIONAL LIBRARIES USED
#-------------------------------

# GLFW
if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
else()
	if(IS_64BIT_BUILD)
		set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.2.1.bin.WIN64")
	else()
		set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.2.1.bin.WIN32")
	endif()
	set(GLFW_INCLUDE_DIRS "${GLFW_ROOT}/include")
	set(GLFW_STATIC_LIBRARIES "${GLFW_ROOT}/lib-vc2015/glfw3.lib")
endif()

# GLAD
set(GLAD_ROOT "${CMAKE_SOURCE_DIR}/dependencies/glad")
set(GLAD_INCLUDE_DIRS "${GLAD_ROOT}/include")
file(GLOB GLAD_SOURCE_FILES "${GLAD_ROOT}/src/glad.c")
file(GLOB GLAD_HEADER_FILES "${GLAD_INCLUDE_DIRS}/glad/glad.h")
message(STATUS "${GLAD_HEADER_FILES}")

# STB
set(STB_ROOT "${CMAKE_SOURCE_DIR}/dependencies/stb")
set(STB_INCLUDE_DIRS "${STB_ROOT}")

# glm
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/glm")

# common_classes
set(COMMON_CLASSES_ROOT "${CMAKE_SOURCE_DIR}/common_classes")
set(COMMON_CLASSES_SHADER_STRUCTS_ROOT "${COMMON_CLASSES_ROOT}/shader_structs")
set(COMMON_CLASSES_STATIC_MESHES_2D_ROOT "${COMMON_CLASSES_ROOT}/static_meshes_2D")
set(COMMON_CLASSES_STATIC_MESHES_2D_PRIMITIVES_ROOT "${COMMON_CLASSES_STATIC_MESHES_2D_ROOT}/primitives")
set(COMMON_CLASSES_STATIC_MESHES_3D_ROOT "${COMMON_CLASSES_ROOT}/static_meshes_3D")
set(COMMON_CLASSES_STATIC_MESHES_3D_PRIMITIVES_ROOT "${COMMON_CLASSES_STATIC_MESHES_3D_ROOT}/primitives")

#-------------------------------
# TUTORIALS SUBDIRECTORIES
#-------------------------------

add_subdirectory(dependencies/windows-freetype2-custom-cmake)

add_subdirectory(001-creating-opengl4-window)
add_subdirectory(002-first-shader)
add_subdirectory(003-adding-colors)
add_subdirectory(004-entering-third-dimension)
add_subdirectory(005-camera-pt1-simple-walking-camera)
add_subdirectory(006-camera-pt2-flying-camera)
add_subdirectory(007-textures-pt1-single-texture)
add_subdirectory(008-textures-pt2-multitexturing)
add_subdirectory(009-orthographic-2D-projection)
add_subdirectory(010-first-great-refactoring)
add_subdirectory(011-indexed-rendering-torus)
add_subdirectory(012-freetype-fonts-pt1)
add_subdirectory(013-blending-pt1)
add_subdirectory(014-normals-diffuse-lighting)
add_subdirectory(015-geometry-shaders)
add_subdirectory(016-heightmap-pt1-random-terrain)

# Output compilation flags
message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})