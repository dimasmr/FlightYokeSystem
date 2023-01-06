# FlightYokeSystem
Logitech/Saitek Flight Yoke System joystick interface using SDL2 API in Linux. **This requires SDL API in Linux. For Windows, see the main branch of this repository. No Simulink/CMEX S-Function implementation**.
Developed on the basis of the samples shown in https://github.com/meleu/jstest-sdl.


Make sure you have installed SDL or SDL2 API https://wiki.libsdl.org/SDL2/Installation. This provides all the necessary headers like SDL.h and its relevant dependency files. You can install via sudo apt: ``` sudo apt-get install libsdl2-dev ```.

Or manually: </br>
``` 
git clone https://github.com/libsdl-org/SDL.git -b SDL2
cd SDL
mkdir build
cd build
../configure
make
sudo make install
```
Make sure you have installed gcc C++ compiler in your system / Visual Studio Code.

Necessary files to be in your folder (or the referenced path):
  1. YokeSystem_linux.h the header file
  2. YokeSystem_linux.cpp the function definition file
  3. Your .cpp impelementation file

**HOW TO COMPILE IN VISUAL STUDIO CODE** </br> 
If you start fresh or tasks.json in .vscode directory is not present. Do the following:
  1. Ctrl+Shitf+D -> Run and Debug. Make sure when you run/debug, choose gcc compiler. You may have errors regarding ```"-fdiagnostics-color=always"``` and unreferenced SDL_Init etc. Abort.
  2. Now the tasks.json file exists. Open it. In the ```"args:"``` field, delete the ```"-fdiagnostics-color=always"```,
     and add the following linkers and necessary arguments: (dont forget to put comma when you add these)
         ```"-lSDL2",
         "-lstdc++", 
         "-std=gnu++11"```

You can read more detailed information in each of the files here.
