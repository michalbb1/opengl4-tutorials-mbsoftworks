#-------------------------------
# GLOBAL SETTINGS
#-------------------------------

cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)

project(opengl4-tutorials-mbsoftworks) # "Solution" name

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "My multi config types" FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#-------------------------------
# ADDITIONAL LIBRARIES USED
#-------------------------------

# GLFW
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(IS_64BIT_BUILD TRUE)
else()
	set(IS_64BIT_BUILD FALSE)
endif()

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
# glm
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dependencies/glm")
# common_classes
set(COMMON_CLASSES_ROOT "${CMAKE_SOURCE_DIR}/common_classes")

#-------------------------------
# GLOBAL VARIABLES
#-------------------------------

# Linkage type - Static or Dynamic
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if(MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

#-------------------------------
# SHARED SOURCES VARIABLES
#-------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

add_subdirectory(001-creating-opengl4-window)

# Output compilation flags
message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})