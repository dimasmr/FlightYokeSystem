// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2023.
// Header file, external dependency file.
// Saitek/Logitech Flight Yoke System.
// Linux and Visual Studio Code with gcc compiler.

// REQUIREMENTS
// A.	Make sure you have installed SDL or SDL2 API https://wiki.libsdl.org/SDL2/Installation
//      This provides all the necessary headers like SDL.h and its relevant dependency files.
//      You can install via sudo apt:
//          -> sudo apt-get install libsdl2-dev
//
//      Or manually:
//          -> git clone https://github.com/libsdl-org/SDL.git -b SDL2
//          -> cd SDL
//          -> mkdir build
//          -> cd build
//          -> ../configure
//          -> make
//          -> sudo make install

// B.   Make sure you have installed gcc C++ compiler in your system / Visual Studio Code.

// C.	Necessary files to be in your folder (or the referenced path):
//			1. YokeSystem_linux.h the header file: this file
//			2. YokeSystem_linux.cpp the function definition file        
// ---------------------------------------------------------------------------------------------------------- //

#ifndef YOKESYSTEM_LINUX_H
#define YOKESYSTEM_LINUX_H

#include <SDL2/SDL.h>   // SDL2 API header, if SDL, change to <SDL.h>
#include <stdio.h>      // For std I/O namespace
#include <iostream>     // For std I/O stream
#include <assert.h>     // For assert

SDL_Window* window = NULL;		// Get the instance of the window, null is fine
SDL_Renderer* renderer = NULL;	// Get the instance of the renderer, null is fine

// Note: the pointer to the interfaces is written not as a struct, as opposed to the Windows version.
// The SDL API is started only once.
// A caveat is, this code has not tested yet for Simulink (CMEX S-Function API)
// To use the header file .h and function definition file .cpp, perhaps it is better to write it like
//      that in the Windows version, see https://github.com/dimasmr/YokeHOTAS

// Global variable, the device count
int deviceCount; 

// The maximum number of connected devices is four
// Every time an SDL object is made, put it in the pointer of pointers.
SDL_Joystick** Yoke_devs[4];     // Pointer to pointers of the joystick input interface, name it Yoke_devs

// Define the structs for multiple joysticks
// For capabilities, manually made
struct JoyCaps
{
    int NumAxes;
    int NumButtons;
    int NumHats;
};

// For states, manually made. The size will be allocated based on capabilities in the implementation
struct JoyStates
{
    Sint16* axes;
    Uint8*  buttons;
    Uint8*  hats;
};

// Define the required values for normalization of the axes
const double thrs = 32767.0f, thrsZ = 65535.0f;
const int deadzone = 3500;  // Deadzone to prevent jumps due to light touch at the origins

class Yoke_linux64 {
public:                     // Acces specifier, public = can be accessed and modified outside the class
    // Class constructors!
    Yoke_linux64();         // Default constructor
    Yoke_linux64(int id);   // Consctructor, accepts arguments, will be defined outside the class via Class::Class( args )

    // Class methods for SDL
    SDL_Joystick* InitDev();
    JoyCaps GetCaps();
    JoyStates GetState();
    int GetButtonNum();
    void UnacqDev();

    // Class methods for state normalization
    double Steer();
	double Nose();
	double BlackClutch();
	double BlueClutch();
	double RedClutch();
    int povdeg();

    // Class methods for buttons
    int IsButtonPressed(int button_id);

private:
    // Class important variables! In private for safety! Comment out the above //private: for debugging!
    SDL_Joystick* theyokes;      // SDL_Joystick structure of SDL API
    JoyCaps caps;           
    JoyStates state;
    int joystick_id; 
};

#endif