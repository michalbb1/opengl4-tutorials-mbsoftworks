# Megabyte Softworks OpenGL4 Tutorials

Welcome to my humble repository with OpenGL4 tutorials! My goal is to transform all the knowledge that I currently possess into multiple tutorials with well-arranged high-quality code, so that you can learn various 3D graphics programming techniques!

Framework that I've chosen is [GLFW](https://www.glfw.org), it seems to be most widely used nowadays. Tutorials are made in a way, that you can easily run them in your favorite environment. On Windows, it's hopefully Visual Studio. I'm planning to add CMake support very soon, so that one can compile and run tutorials on Linux too (or even build solution file out of it on Windows).

To find the list of all tutorials along with the related articles and in-depth explanations, click here:

#### [Megabyte Softworks OpenGL4 Tutorials](http://www.mbsoftworks.sk/tutorials/opengl4 "Homepage of OpenGL4 tutorials")

# How to compile on Windows

Tutorials are designed to run out-of-the-box of Windows. Just clone the repository somewhere and all of the related dependencies (linked as git submodules). Open your command line processor (Git Bash, cmd) and enter following commands:

```
git clone https://github.com/michalbb1/opengl4-tutorials-mbsoftworks.git
cd opengl4-tutorials-mbsoftworks
git submodule update --init --recursive
```

First command clone the repository, second changes working directory to it and the third downloads source codes of all dependendcies (might take a while).

After that, just open provided Solution file ```opengl4-tutorials-mbsoftworks-vs2017.sln``` (Visual Studio 2017) solution and Build it (Ctrl+Shift+B usually). That's it!

