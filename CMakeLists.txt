#-------------------------------
# GLOBAL SETTINGS
#-------------------------------

cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)

project(opengl4-tutorials-mbsoftworks) # "Solution" name

#-------------------------------
# GLOBAL VARIABLES
#-------------------------------

if(MSVC)
	message(STATUS "Using Visual Studio as compiler, version ${MSVC_VERSION}")
	if(${MSVC_VERSION} GREATER_EQUAL 1900 AND ${MSVC_VERSION} LESS_EQUAL 1919)
		set(VS_PREBUILT_FOLDER_NAME "vs2017")
		set(FREETYPE2_LIB_NAME_32 "freetype2.lib")
		set(FREETYPE2_LIB_NAME_64 "freetype2_64.lib")
		set(ASSIMP_DEBUG_LIB_NAME "assimp-vc141-mtd.lib")
		set(ASSIMP_RELEASE_LIB_NAME "assimp-vc141-mt.lib")
	elseif(${MSVC_VERSION} GREATER_EQUAL 1920 AND ${MSVC_VERSION} LESS_EQUAL 1929)
		set(VS_PREBUILT_FOLDER_NAME "vs2019")
		set(FREETYPE2_LIB_NAME_32 "freetype.lib")
		set(FREETYPE2_LIB_NAME_64 "freetype.lib")
		set(ASSIMP_DEBUG_LIB_NAME "assimp-vc142-mtd.lib")
		set(ASSIMP_RELEASE_LIB_NAME "assimp-vc142-mt.lib")
	else()
		message(FATAL_ERROR "Unsupported Visual Studio version! Supported Visual Studio version are 2017 / 2019!")
	endif()
	message(STATUS "Name of folder for prebuilt libraries: ${VS_PREBUILT_FOLDER_NAME}")
	message(STATUS "FreeType2 library name for 32-bit version: ${FREETYPE2_LIB_NAME_32}")
	message(STATUS "FreeType2 library name for 64-bit version: ${FREETYPE2_LIB_NAME_64}")
	message(STATUS "Assimp debug library name: ${ASSIMP_DEBUG_LIB_NAME}")
	message(STATUS "Assimp release library name: ${ASSIMP_RELEASE_LIB_NAME}")
endif()

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
	message(STATUS "Build is 64-bit")
else()
	set(IS_64BIT_BUILD FALSE)
	message(STATUS "Build is 32-bit")
endif()

# Log if it's debug or release configuration
if(CMAKE_BUILD_TYPE EQUAL "Debug")
	message(STATUS "Build configuration is debug")
	set(IS_DEBUG_BUILD TRUE)
else()
	message(STATUS "Build configuration is release")
	set(IS_DEBUG_BUILD FALSE)
endif()

#-------------------------------
# ADDITIONAL LIBRARIES USED
#-------------------------------

# GLFW
if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
else()
	set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/glfw/include")
	if(IS_64BIT_BUILD)
		set(GLFW_STATIC_LIBRARIES "${CMAKE_SOURCE_DIR}/dependencies/glfw-prebuilt/${VS_PREBUILT_FOLDER_NAME}/libs64/glfw3.lib")
	else()
		set(GLFW_STATIC_LIBRARIES "${CMAKE_SOURCE_DIR}/dependencies/glfw-prebuilt/${VS_PREBUILT_FOLDER_NAME}/libs32/glfw3.lib")
	endif()
endif()

message(STATUS "GLFW include directories: ${GLFW_INCLUDE_DIRS}")
message(STATUS "GLFW static libraries: ${GLFW_STATIC_LIBRARIES}")

# GLAD
set(GLAD_ROOT "${CMAKE_SOURCE_DIR}/dependencies/glad")
set(GLAD_INCLUDE_DIRS "${GLAD_ROOT}/include")
file(GLOB GLAD_SOURCE_FILES "${GLAD_ROOT}/src/glad.c")
file(GLOB GLAD_HEADER_FILES "${GLAD_INCLUDE_DIRS}/glad/glad.h")

message(STATUS "GLAD include directories: ${GLAD_INCLUDE_DIRS}")

# STB
set(STB_ROOT "${CMAKE_SOURCE_DIR}/dependencies/stb")
set(STB_INCLUDE_DIRS "${STB_ROOT}")

message(STATUS "STB include directories: ${GLAD_INCLUDE_DIRS}")

# glm
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/glm")

message(STATUS "GLM include directories: ${GLM_INCLUDE_DIRS}")

# FreeType2
if(UNIX)
	pkg_search_module(FREETYPE REQUIRED freetype2)
else()
	set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/freetype2/include")
	if(IS_64BIT_BUILD)
		set(FREETYPE_STATIC_LIBRARIES "${CMAKE_SOURCE_DIR}/dependencies/freetype2-prebuilt/${VS_PREBUILT_FOLDER_NAME}/libs64/${FREETYPE2_LIB_NAME_64}")
	else()
		set(FREETYPE_STATIC_LIBRARIES "${CMAKE_SOURCE_DIR}/dependencies/freetype2-prebuilt/${VS_PREBUILT_FOLDER_NAME}/libs32/${FREETYPE2_LIB_NAME_32}")
	endif()
endif()

message(STATUS "FreeType2 include directories: ${FREETYPE_INCLUDE_DIRS}")
message(STATUS "FreeType2 libraries: ${FREETYPE_STATIC_LIBRARIES}")

# Assimp
if(UNIX)
	pkg_search_module(ASSIMP REQUIRED assimp)
else()
	set(ASSIMP_PREBUILT_ROOT "${CMAKE_SOURCE_DIR}/dependencies/assimp-prebuilt")
	set(ASSIMP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/assimp/include;${ASSIMP_PREBUILT_ROOT}/include")
	if(IS_64BIT_BUILD)
		if(IS_DEBUG_BUILD)
			set(ASSIMP_LIBS_ROOT "${ASSIMP_PREBUILT_ROOT}/${VS_PREBUILT_FOLDER_NAME}/libs64/debug")
			set(ASSIMP_STATIC_LIBRARIES "${ASSIMP_LIBS_ROOT}/${ASSIMP_DEBUG_LIB_NAME};${ASSIMP_LIBS_ROOT}/IrrXMLd.lib;${ASSIMP_LIBS_ROOT}/zlibstaticd.lib")
		else()
			set(ASSIMP_LIBS_ROOT "${ASSIMP_PREBUILT_ROOT}/${VS_PREBUILT_FOLDER_NAME}/libs64/release")
			set(ASSIMP_STATIC_LIBRARIES "${ASSIMP_LIBS_ROOT}/${ASSIMP_RELEASE_LIB_NAME};${ASSIMP_LIBS_ROOT}/IrrXML.lib;${ASSIMP_LIBS_ROOT}/zlibstatic.lib")
		endif()
		set(ASSIMP_LIBS_ROOT "${ASSIMP_PREBUILT_ROOT}/${VS_PREBUILT_FOLDER_NAME}/libs64")
	else()
		if(IS_DEBUG_BUILD)
			set(ASSIMP_LIBS_ROOT "${ASSIMP_PREBUILT_ROOT}/${VS_PREBUILT_FOLDER_NAME}/libs32/debug")
			set(ASSIMP_STATIC_LIBRARIES "${ASSIMP_LIBS_ROOT}/${ASSIMP_DEBUG_LIB_NAME};${ASSIMP_LIBS_ROOT}/IrrXMLd.lib;${ASSIMP_LIBS_ROOT}/zlibstaticd.lib")
		else()
			set(ASSIMP_LIBS_ROOT "${ASSIMP_PREBUILT_ROOT}/${VS_PREBUILT_FOLDER_NAME}/libs32/release")
			set(ASSIMP_STATIC_LIBRARIES "${ASSIMP_LIBS_ROOT}/${ASSIMP_RELEASE_LIB_NAME};${ASSIMP_LIBS_ROOT}/IrrXML.lib;${ASSIMP_LIBS_ROOT}/zlibstatic.lib")
		endif()
	endif()
endif()

message(STATUS "Assimp include directories: ${ASSIMP_INCLUDE_DIRS}")
message(STATUS "Assimp libraries: ${ASSIMP_STATIC_LIBRARIES}")

# Variables for common classes (used throughout all projects)
set(COMMON_CLASSES_ROOT "${CMAKE_SOURCE_DIR}/common_classes")
set(COMMON_CLASSES_SHADER_STRUCTS_ROOT "${COMMON_CLASSES_ROOT}/shader_structs")
set(COMMON_CLASSES_STATIC_MESHES_2D_ROOT "${COMMON_CLASSES_ROOT}/static_meshes_2D")
set(COMMON_CLASSES_STATIC_MESHES_2D_PRIMITIVES_ROOT "${COMMON_CLASSES_STATIC_MESHES_2D_ROOT}/primitives")
set(COMMON_CLASSES_STATIC_MESHES_3D_ROOT "${COMMON_CLASSES_ROOT}/static_meshes_3D")
set(COMMON_CLASSES_STATIC_MESHES_3D_PRIMITIVES_ROOT "${COMMON_CLASSES_STATIC_MESHES_3D_ROOT}/primitives")

#-------------------------------
# TUTORIALS SUBDIRECTORIES
#-------------------------------

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
add_subdirectory(017-heightmap-pt2-from-image-and-skybox)
add_subdirectory(018-heightmap-pt3-multiple-layers)
add_subdirectory(019-assimp-model-loading)
add_subdirectory(020-fog)
add_subdirectory(021-specular-highlight)
add_subdirectory(022-cylinder-and-sphere)
add_subdirectory(023-point-lights)
add_subdirectory(024-uniform-buffer-object)
add_subdirectory(025-transform-feedback-particle-system)
add_subdirectory(026-camera-pt3-orbit-camera)

# Output compilation flags
message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})