# Greedy Delvers

Welcome to the WIP game project with the codename Greedy Delvers.

## Setting up the environment 

Here are some simple steps to follow in order to get ready to start developing. Windows only.

**1.** Begin with downloading MSYS2 and installing it. 

**2.** Now open the program MSYS2 UCRT64 and writre the following command below using the default installation and picking yes.
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake
```

**3.** Go to you windows search bar and open *Edit the system environment variables*, go to *Environment variables*, select *path* and click edit and add *C:\msys64\ucrt64\bin* or a diffrent directory if you chose it during installation. 

**4.** Create a folder for your project and as a first try put the *hello.c* file along with the *CMakeLists.txt* in the folder, files can be found [here](https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md).

**5.** Now go to Visual Code and open the previusly mentioned project folder. Open the terminal and write the four commands below seperately in order (for all libraries).
```
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
git clone https://github.com/libsdl-org/SDL_image.git vendored/SDL_image
git clone https://github.com/libsdl-org/SDL_net.git vendored/SDL_net
git clone https://github.com/libsdl-org/SDL_mixer.git vendored/SDL_mixer
```
Keep in mind that ttf also exists and can be used with the command down below but has issues with adding vendored repos.
```
git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf
```
6. Now use the commands below seperately below in order to compile.
```
cmake -S . -B build
cmake --build build
cd build
./hello
```

**7.** If you previous step gave you a *Hello World* in a window you can now clone the repo and open it in Visual Code and start programming.

## Visual Code Syntax 
Extra steps if you want syntax handling in Visual Studio Code.

**1.** Enable or install the C/C++ extension in Visual Code.

**2.** Open your project in Visual Code and after that select the top search bar. Search and select *Show and Run Commands*, then *C/C++: Select a Configuration*, select *Edit Configuration (JSON)*.

**3.** You should now have a .vscode folder in your project with a file named *c_cpp_properties.json*. In that file under includepath and below *workspacefolder* add the code below, remeber to add a comma after workspacefolder. 
```
"${workspaceFolder}/vendored/SDL/include"
```
For all the other major SDL libraries do the same, e.g. *SDL_image*.
