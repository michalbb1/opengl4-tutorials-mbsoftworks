#-------------------------------
# FREETYPE CONFIGURATION
#-------------------------------

set(PROJECT_NAME "freetype")
project(${PROJECT_NAME}) # Project's name
cmake_minimum_required(VERSION 3.1) # Specify the minimum version for CMake
set (CMAKE_CXX_STANDARD 14) # Support C++14

set(FREETYPE_ROOT "${CMAKE_SOURCE_DIR}/dependencies/freetype2")
set(FREETYPE_SRC_ROOT "${FREETYPE_ROOT}/src")

set(FREETYPE_CUSTOM_CMAKE_CPP_FILES
"${FREETYPE_SRC_ROOT}/autofit/autofit.c"
"${FREETYPE_SRC_ROOT}/base/ftbase.c"
"${FREETYPE_SRC_ROOT}/base/ftbbox.c"
"${FREETYPE_SRC_ROOT}/base/ftbdf.c"
"${FREETYPE_SRC_ROOT}/base/ftbitmap.c"
"${FREETYPE_SRC_ROOT}/base/ftcid.c"
"${FREETYPE_SRC_ROOT}/base/ftdebug.c"
"${FREETYPE_SRC_ROOT}/base/ftfstype.c"
"${FREETYPE_SRC_ROOT}/base/ftgasp.c"
"${FREETYPE_SRC_ROOT}/base/ftglyph.c"
"${FREETYPE_SRC_ROOT}/base/ftgxval.c"
"${FREETYPE_SRC_ROOT}/base/ftinit.c"
"${FREETYPE_SRC_ROOT}/base/ftmm.c"
"${FREETYPE_SRC_ROOT}/base/ftsystem.c"
"${FREETYPE_SRC_ROOT}/bdf/bdf.c"
"${FREETYPE_SRC_ROOT}/cff/cff.c"
"${FREETYPE_SRC_ROOT}/cid/type1cid.c"
"${FREETYPE_SRC_ROOT}/gzip/ftgzip.c"
"${FREETYPE_SRC_ROOT}/lzw/ftlzw.c"
"${FREETYPE_SRC_ROOT}/pcf/pcf.c"
"${FREETYPE_SRC_ROOT}/pfr/pfr.c"
"${FREETYPE_SRC_ROOT}/psaux/psaux.c"
"${FREETYPE_SRC_ROOT}/pshinter/pshinter.c"
"${FREETYPE_SRC_ROOT}/psnames/psnames.c"
"${FREETYPE_SRC_ROOT}/raster/raster.c"
"${FREETYPE_SRC_ROOT}/sfnt/sfnt.c"
"${FREETYPE_SRC_ROOT}/smooth/ftgrays.c"
"${FREETYPE_SRC_ROOT}/smooth/ftsmooth.c"
"${FREETYPE_SRC_ROOT}/truetype/truetype.c"
"${FREETYPE_SRC_ROOT}/type1/type1.c"
"${FREETYPE_SRC_ROOT}/type42/type42.c"
"${FREETYPE_SRC_ROOT}/winfonts/winfnt.c"
)

add_definitions(-DFT2_BUILD_LIBRARY)
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_ROOT}/include" CACHE STRING "Freetype2 include directories")

# Setup include directories of the FreeType
include_directories("${FREETYPE_INCLUDE_DIRS}")

# Add library
add_library(${PROJECT_NAME} STATIC ${FREETYPE_CUSTOM_CMAKE_CPP_FILES})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${LIBRARY_OUTPUT_PATH}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${LIBRARY_OUTPUT_PATH}"
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W1)
endif()

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
