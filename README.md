# Dungeons-and-Greed

Welcome to the WIP game project with the codename Dungeons and Greed.

## Setting up the environment 

Here are some simple steps to follow in order to get ready to start developing. Windows only.

**1.** Begin with downloading MSYS2 and installing it. 

**2.** Now open the program MSYS2 UCRT64 and writre the following command below using the default installation and picking yes.

```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-sdl3
```

**3.** Create a folder for your project and as a first try put the *hello.c* file along with the *CMakeLists.txt* in the folder, files can be found [here](https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-mingw.md).

**4.** Go to you windows search bar and open *Edit the system environment variables*, go to *Environment variables*, select *path* and click edit and add *C:\msys64\ucrt64\bin* or a diffrent directory if you chose it during installation. 

**5.** Now go to Visual Code and open the previusly mentioned project folder. Open the terminal and write the four commands below seperately in order.

```
cmake -S . -B build
cmake --build build
cd build
./hello
```

**6.** If you previous step gave you a *Hello World* in a window you can now clone the repo and open it in Visual Code and start programming.
