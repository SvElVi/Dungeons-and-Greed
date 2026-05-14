# Greedy Delvers

Welcome to the WIP game project with the codename Greedy Delvers. When using this repository, only setting up the environment is necessary.

## Setting up the environment 

Here are some simple steps to follow in order to get ready to start developing on this project. Windows only.

**1.** Begin with downloading MSYS2 and installing it. 

**2.** Now open the program MSYS2 UCRT64 and writre the following command below using the default installation and picking yes.
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-freetype
```

**3.** Go to you windows search bar and open *Edit the system environment variables*, go to *Environment variables*, select *path* and click edit and add *C:\msys64\ucrt64\bin* or a diffrent directory if you chose it during installation. 

**4.** Clone this repository with a git program of your choice and remember the place where you put the project folder for your next step.

**5.** Now go to Visual Code and open the previusly mentioned project folder. Open the terminal and write the four commands below seperately in order (for all libraries). This will create vendored folder inside the cloned repo.
```
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
git clone https://github.com/libsdl-org/SDL_net.git vendored/SDL_net
```
Keep in mind that image,ttf and mixer also exists and can be used with the command down below but has issues with adding vendored repos.
```
git clone https://github.com/libsdl-org/SDL_image.git vendored/SDL_image
git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf
git clone https://github.com/libsdl-org/SDL_mixer.git vendored/SDL_mixer
```
To fix this you will need to download the dependencies for image, ttf and mixer with commands in windows powershell admin mode, the executeble files can be found in *vendored/SDL_[ttf or mixer]/external/Get-GitModules.ps1*. Remember to jump to the files with *cd [path]* in powershell and execute the *.ps1* file with *./[path]*. To allow windows to automatically download the dependencies you will need to execute the command below and it will work whilst you have the window up.
```
Set-ExecutionPolicy RemoteSigned -Scope Process
```

**6.** Now use the commands below seperately below in order to compile in Visual Code. In the future you will only need to use the last two commands to compile unless you decide to add additional libraries.
```
cd src/client
cmake -S . -B build
cd ../server
cmake -S . -B build
cd ../..
cmake --build src/server/build
cmake --build src/client/build
```

**7.** To run the game simply open run the compiled files. You can do this in your termianl using the following commands for the respective part of the game.
```
./src/client/build/client
./src/server/build/server
```

## Visual Code Syntax 
Extra steps if you want syntax handling in Visual Studio Code.

**1.** Enable or install the C/C++ extension in Visual Code.

**2.** Open your project in Visual Code and after that select the top search bar. Search and select *Show and Run Commands*, then *C/C++: Select a Configuration*, select *Edit Configuration (JSON)*.

**3.** You should now have a .vscode folder in your project with a file named *c_cpp_properties.json*. In that file under includepath and below *workspacefolder* add the code below, remeber to add a comma after workspacefolder. 
```
"${workspaceFolder}/vendored/SDL/include"
```
For all the other major SDL libraries do the same, e.g. *SDL_image*.

**4.** It is possible you still have error squiggles. To fix these you can go to *Show and Run Commands*, then *C/C++: Select intelliSence Configuration* and select *gcc.exe*.
