#!/bin/bash

# First, download all submodules and repositories
echo "Updating all submodules..."
git submodule update --init --recursive

# FreeType2 pre-built libraries downloads
echo "Creating subdirectories for FreeType2 libraries..."
mkdir -p "dependencies/freetype2-prebuilt/vs2017/libs32"
mkdir -p "dependencies/freetype2-prebuilt/vs2017/libs64"

echo "Downloading pre-built FreeType2 library for VS2017 (32-bit version)..."
curl -o "dependencies/freetype2-prebuilt/vs2017/libs32/freetype2.lib" "http://www.mbsoftworks.sk/meta/prebuilt/freetype2/vs2017/libs32/freetype2.lib"
echo "Downloading pre-built FreeType2 library for VS2017 (64-bit version)..."
curl -o "dependencies/freetype2-prebuilt/vs2017/libs64/freetype2_64.lib" "http://www.mbsoftworks.sk/meta/prebuilt/freetype2/vs2017/libs64/freetype2_64.lib"

echo "Creating subdirectories for Assimp libraries..."
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs32/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs32/release"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs64/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs64/release"

# Assimp pre-built libraries downloads
echo "Downloading Assimp pre-built libraries for VS2017 / 32-bit debug version..."
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/assimp-vc141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/assimp-vc141-mtd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/IrrXMLd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/zlibstaticd.lib"

echo "Downloading Assimp pre-built libraries for VS2017 / 64-bit debug version..."
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/assimp-vc141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/assimp-vc141-mtd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/IrrXMLd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/zlibstaticd.lib"

echo "Downloading Assimp pre-built libraries for VS2017 / 32-bit release version..."
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/assimp-vc141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/assimp-vc141-mt.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/IrrXML.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/zlibstatic.lib"

echo "Downloading Assimp pre-built libraries for VS2017 / 64-bit release version..."
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/assimp-vc141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/assimp-vc141-mt.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/IrrXML.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/zlibstatic.lib"