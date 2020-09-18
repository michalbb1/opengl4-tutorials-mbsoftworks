# Megabyte Softworks OpenGL4 Tutorials

Welcome to my humble repository with OpenGL4 tutorials! My goal is to transform all the knowledge that I currently possess into multiple tutorials with well-arranged high-quality code, so that you can learn various 3D graphics programming techniques!

Framework that I've chosen is [GLFW](https://www.glfw.org), it seems to be most widely used nowadays. I've tried to make the tutorials in such a way, that you should have no trouble running them both on Windows or Unix systems (I have tested them on Ubuntu 18.04 and everything has built just fine). Just don't skip this README document and you will surely have no trouble bringing the tutorials to life at all!

To find the list of all tutorials along with the related articles and in-depth explanations, click on the link below:

### [Megabyte Softworks OpenGL4 Tutorials Website](http://www.mbsoftworks.sk/tutorials/opengl4 "Homepage of OpenGL4 tutorials")

There are also pre-built zipped version for Windows for a quick preview of all tutorials.

# How to setup and run on Windows with Visual Studio

Tutorials are designed to run out-of-the-box on Windows. Just clone / download the repository somewhere. Then open this fresh clone in some command line processor, ideally Git Bash or Poweshell (just don't use plain old `cmd.exe`) and then run script `windows_setup.sh`.

This will initiate cloning of all needed submodules and will download some pre-built libraries to your repository from my server (otherwise you won't be able to compile some of the tutorials at all). So the whole setup of the tutorials consists of following three lines in your command line processor:

```
git clone https://github.com/michalbb1/opengl4-tutorials-mbsoftworks.git

cd opengl4-tutorials-mbsoftworks

./windows_setup.sh
```

When it's done, just open provided Solution file `opengl4-tutorials-mbsoftworks-vs2017.sln` (Visual Studio 2017) solution and Build it (Ctrl+Shift+B usually) and you are good to go!

If the Visual Studio complains about wrong Windows SDK version, just change it per tutorial to the SDK you have or install one that is used for the tutorials, that is **`10.0.17134.0`** (you can do it via **Control Panel / Programs and Features / Visual Studio 2017 / Change** and then somewhere under C++ you can add this Windows SDK component). Unfortunately, in VS2017 it is not possible to just choose the available SDK (this has been addressed in VS2019, support of which I plan soon).

# How to setup and run on Windows with CMake

CMake is a cross-platform system that helps you to manage the build process among various operating systems and compilers. If you're not familiar with CMake, I really recommend you not to be afraid of it, as it's super easy to use and almost all C++ projects are built using CMake!

First you will need to download and install CMake. You can find it here:

### [CMake Website](https://cmake.org/ "CMake Website")

Now that you have it, you can clone / download the repository and then you still need to run provided script `windows_setup.sh`. When it finishes, you can build a CMake project now. Here is the whole procedure again:

```
git clone https://github.com/michalbb1/opengl4-tutorials-mbsoftworks.git

cd opengl4-tutorials-mbsoftworks

./windows_setup.sh

mkdir build

(cd build && cmake ..)
```

After that, you should find a generated solution / project files in the `build` folder (depending on the generator, but default one for Windows is Visual Studio)/ Open and build it and that should be it!

# How to setup and run on Unix systems with CMake

Because the tutorials are standard CMake project, building them involves a standard procedure of building CMake projects. However because you might be missing some of the required packages / libraries, I have prepared a script called `unix_setup.sh`, which should download everything that the tutorials need. So the whole procedure for building the tutorials looks like this:

```
git clone https://github.com/michalbb1/opengl4-tutorials-mbsoftworks.git

cd opengl4-tutorials-mbsoftworks

chmod +x unix_setup.sh

sudo ./unix_setup.sh # Run this with sudo! Otherwise the packages won't install

mkdir build

(cd build && cmake .. && make -j4)
```

This process will take a while, but once it finishes, the compiled tutorials can be found in the `bin` folder. That's it!

I've tested this with freshly installed Ubuntu and using the exact steps as described here has worked just fine.

# Troubleshooting

It is possible that despite my effort some of you might have trouble compiling and running the tutorials. This might happen - I'm just a human and I make mistakes, also I am not capable of testing all possible operating systems / compiler combinations and maintain it.

If you still have some trouble making the tutorials run, the easiest way is to contact me on my e-mail `michalbb1@gmail.com` or by some other means, like through my [Facebook Page](https://www.facebook.com/MegabyteSoftworks/ "Megabyte Softworks Facebook Page"). I will try my best to help you.

# List of libraries used
It would be a madness to program every single thing from a scratch (math, utilities, image loading etc...), that's why my tutorials use other libraries as well (mostly as git submodules). Here is a list of all of them:

- [GLFW](https://www.glfw.org) - OpenGL framework used for cross-platform window creation and input handling
- [GLM](https://glm.g-truc.net/0.9.9/index.html) - without doubt most widely mathematics library used when using OpenGL (vectors, matrices, quaternions etc.)
- [STB](https://github.com/nothings/stb) - header-only library with several very useful functions like loading an image from a file (supports most common formats)
- [The FreeType Project](https://www.freetype.org/) - used for manipulating fonts used even in some Linux distributions, Android or iOS. My tutorials use it to load / display text
- [Assimp](http://www.assimp.org) - used to load and handle 3D model data. Supports most of the commonly used 3D model file formats

