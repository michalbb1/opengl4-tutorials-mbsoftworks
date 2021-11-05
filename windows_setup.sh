#!/bin/bash

# First, download all submodules and repositories
echo "Updating all submodules..."
git submodule update --init --recursive

# Store the root for pre-built files in a variable
prebuilt_root="https://www.mbsoftworks.sk/meta/prebuilt"

# FreeType2 pre-built libraries downloads
prebuilt_root_freetype="${prebuilt_root}/freetype2/vs2019"
echo "Creating subdirectories for FreeType2 libraries..."
mkdir -p "dependencies/freetype2-prebuilt/vs2019/libs32"
mkdir -p "dependencies/freetype2-prebuilt/vs2019/libs64"

echo "Downloading pre-built FreeType2 library for VS2019 (32-bit version)..."
curl -k -o "dependencies/freetype2-prebuilt/vs2019/libs32/freetype.lib" "${prebuilt_root_freetype}/libs32/freetype.lib"
echo "Downloading pre-built FreeType2 library for VS2019 (64-bit version)..."
curl -k -o "dependencies/freetype2-prebuilt/vs2019/libs64/freetype.lib" "${prebuilt_root_freetype}/libs64/freetype.lib"

echo "Creating subdirectories for Assimp libraries..."
mkdir -p "dependencies/assimp-prebuilt/vs2019/libs32/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2019/libs32/release"
mkdir -p "dependencies/assimp-prebuilt/vs2019/libs64/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2019/libs64/release"

# Assimp pre-built libraries downloads
prebuilt_root_assimp="${prebuilt_root}/assimp/vs2019"
echo "Downloading Assimp pre-built libraries for VS2019 / 32-bit debug version..."
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/debug/assimp-vc142-mtd.lib" "${prebuilt_root_assimp}/libs32/debug/assimp-vc142-mtd.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/debug/IrrXMLd.lib" "${prebuilt_root_assimp}/libs32/debug/IrrXMLd.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/debug/zlibstaticd.lib" "${prebuilt_root_assimp}/libs32/debug/zlibstaticd.lib"

echo "Downloading Assimp pre-built libraries for VS2019 / 64-bit debug version..."
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/debug/assimp-vc142-mtd.lib" "${prebuilt_root_assimp}/libs64/debug/assimp-vc142-mtd.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/debug/IrrXMLd.lib" "${prebuilt_root_assimp}/libs64/debug/IrrXMLd.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/debug/zlibstaticd.lib" "${prebuilt_root_assimp}/libs64/debug/zlibstaticd.lib"

echo "Downloading Assimp pre-built libraries for VS2019 / 32-bit release version..."
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/release/assimp-vc142-mt.lib" "${prebuilt_root_assimp}/libs32/release/assimp-vc142-mt.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/release/IrrXML.lib" "${prebuilt_root_assimp}/libs32/release/IrrXML.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs32/release/zlibstatic.lib" "${prebuilt_root_assimp}/libs32/release/zlibstatic.lib"

echo "Downloading Assimp pre-built libraries for VS2019 / 64-bit release version..."
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/release/assimp-vc142-mt.lib" "${prebuilt_root_assimp}/libs64/release/assimp-vc142-mt.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/release/IrrXML.lib" "${prebuilt_root_assimp}/libs64/release/IrrXML.lib"
curl -k -o "dependencies/assimp-prebuilt/vs2019/libs64/release/zlibstatic.lib" "${prebuilt_root_assimp}/libs64/release/zlibstatic.lib"