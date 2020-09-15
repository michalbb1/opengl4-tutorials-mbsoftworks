#!/bin/bash

# First, download all submodules and repositories
echo "Downloading all the submodules / repositories..."
git submodule update --init --recursive

# Install pkg-config for CMake to work properly
echo "Installing pkg-config..."
apt-get install -y pkg-config

# Install doxygen - needed for build & install of GLFW
echo "Installing doxygen..."
apt-get install -y doxygen

# Install xorg-dev - needed for build & install of GLFW
echo "Installing xorg-dev..."
apt-get install -y xorg-dev

# Now we can build and install GLFW
echo "Building and installing GLFW..."
mkdir -p dependencies/glfw/build
(cd dependencies/glfw/build && cmake .. && make -j4 && make install)

# Now go back to the root folder and now we can finally build the tutorials!
echo "Building tutorials..."
mkdir build
(cd build && cmake .. && make -j4)