# Megabyte Softworks OpenGL4 Tutorials

Welcome to my humble repository with OpenGL4 tutorials! My goal is to transform all the knowledge that I currently possess into multiple tutorials with well-arranged high-quality code, so that you can learn various 3D graphics programming techniques!

Framework that I've chosen is [GLFW](https://www.glfw.org), it seems to be most widely used nowadays. Tutorials are made in a way, that you can easily run them in your favorite environment. On Windows, it's hopefully Visual Studio.

CMake support is there already, it's been tested on Windows, but not on Linux yet (some adjusments have to be made for sure).

To find the list of all tutorials along with the related articles and in-depth explanations, click here:

##### [Megabyte Softworks OpenGL4 Tutorials](http://www.mbsoftworks.sk/tutorials/opengl4 "Homepage of OpenGL4 tutorials")

# How to compile on Windows

Tutorials are designed to run out-of-the-box on Windows. Just clone / download the repository somewhere. Then open this fresh clone in some command line processor, ideally Git Bash or Poweshell and then run script `download_prerequisities.sh`. This will initiate cloning of all needed submodules and will download some pre-built libraries to your repository (otherwise you won't be able to compile at all). So the whole setup of the tutorials consists of following three lines in your command line processor:

```
git clone https://github.com/michalbb1/opengl4-tutorials-mbsoftworks.git
cd opengl4-tutorials-mbsoftworks
./download_prerequisities.sh
```

First command clones the repository, second changes working directory to it and the third downloads all of the dependencies (might take a while).

After that, just open provided Solution file `opengl4-tutorials-mbsoftworks-vs2017.sln` (Visual Studio 2017) solution and Build it (Ctrl+Shift+B usually). That's it!

# List of libraries used
It would be a madness to program every single thing from a scratch (math, utilities, image loading etc...), that's why my tutorials use other libraries as well (mostly as git submodules). Here is a list of all of them:

- [GLFW](https://www.glfw.org) - OpenGL framework used for cross-platform window creation and input handling
- [GLM](https://glm.g-truc.net/0.9.9/index.html) - without doubt most widely mathematics library used when using OpenGL (vectors, matrices, quaternions etc.)
- [STB](https://github.com/nothings/stb) - header-only library with several very useful functions like loading an image from a file (supports most common formats)
- [The FreeType Project](https://www.freetype.org/) - used for manipulating fonts used even in some Linux distributions, Android or iOS. My tutorials use it to load / display text
- [Assimp](http://www.assimp.org) - used to load and handle 3D model data. Supports most of the commonly used 3D model file formats