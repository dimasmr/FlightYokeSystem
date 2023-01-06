// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2023.
// Functions definitions file. Requires YokeSystem_linux.h header!
// This file is not a main file (implementation), you need to make your own implementation file!
// Nevertheless, a sample is provided, namely LINUX_TESTYokeSystem.cpp.
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
//			1. YokeSystem_linux.h the header file
//			2. YokeSystem_linux.cpp the function definition file: this file
//			3. Your .cpp impelementation file

// HOW TO COMPILE IN VISUAL STUDIO CODE USING GCC
// If you start fresh or tasks.json in .vscode directory is not present. Do the following:
//  1. Ctrl+Shitf+D -> Run and Debug.
//     Make sure when you run/debug, choose gcc compiler.
//     You may have errors regarding "-fdiagnostics-color=always" and unreferenced SDL_Init etc. Abort.
//  2. Now the tasks.json file exists. Open it. In the "args:" field, delete the "-fdiagnostics-color=always",
//     and add the following linkers and necessary arguments: (dont forget to put comma when you add these)
//          "-lSDL2",
//          "-lstdc++",
//          "-std=gnu++11"          
// ---------------------------------------------------------------------------------------------------------- //

#include "YokeSystem_linux.h"           // External dependency file, header file

Yoke_linux64::Yoke_linux64() {      // To instantiate a Class object, default
    joystick_id = 0;    // Default set joystick ID as 0
    InitDev();          // Initialize the device: every time a class object is made
    GetCaps();          // Get caps
}

Yoke_linux64::Yoke_linux64(int id) {    // To instantiate a Class object, default
    joystick_id = id;   // Set manual joystick ID
    InitDev();          // Initialize the device: every time a class object is made
    GetCaps();          // Get caps
}

// Initialize the SDL API and get the number of connected joystick, do just once
int InitSDL() {
    // Initialize the SDL API interface with several subsystems.
    // https://wiki.libsdl.org/SDL2/SDL_Init
    // For Joysticks, video and joystick are required. They automatically start the event subsystem.
    // One cas use SDL_EVERYTHING to start everything.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Take the number of connected joystick, aka Enumerate.
    // https://wiki.libsdl.org/SDL2/SDL_NumJoysticks
    deviceCount = SDL_NumJoysticks();

    if (deviceCount < 0) {
        printf("No joysticks are found!\n");
    }
 
}

// Initialize and create the device, also do it just once
SDL_Joystick* Yoke_linux64::InitDev() {
    // Put the SDL object in the pointer that points to an SDL_Joystick struct
    // https://wiki.libsdl.org/SDL2/SDL_JoystickOpen
    theyokes = SDL_JoystickOpen(joystick_id);
    
    if (theyokes == NULL) {
        printf( "Unable to open the joystick: %s\n", SDL_GetError());
    }

    // The maximum number of connected devices is four
    // Every time an SDL object is made, put it in the pointer of pointers.
    Yoke_devs[joystick_id] = &theyokes;

    return theyokes;
}

// Get device capabilities, also do it just once
JoyCaps Yoke_linux64::GetCaps() {
    caps = { 0 };   // Initialization of the struct, namely caps with JoyCaps struct

    // Get the capabilities
    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/joystick.html
    caps.NumAxes    = SDL_JoystickNumAxes(theyokes);
    caps.NumButtons = SDL_JoystickNumButtons(theyokes);
    caps.NumHats    = SDL_JoystickNumHats(theyokes);

    // We shall allocate the states once, so call this method once!
    // Cast to the appropriate type defined in the header file
    state = { 0 };

    state.axes     = (Sint16*)calloc(caps.NumAxes, sizeof(Sint16));
    state.buttons  = (Uint8*)calloc(caps.NumButtons, sizeof(Uint8));
    state.hats     = (Uint8*)calloc(caps.NumHats, sizeof(Uint8));
}

// Get button number from caps
int Yoke_linux64::GetButtonNum() {
    return caps.NumButtons;
}

// Get the state from the device, do it every step, use the recommended event system
JoyStates Yoke_linux64::GetState() {
    // Use event system as recommended by the SDL website
    // https://libsdl.org/release/SDL-1.2.15/docs/html/joystick.html
    // https://libsdl.org/release/SDL-1.2.15/docs/html/sdlevent.html
    // https://wiki.libsdl.org/SDL2/SDL_Event
    SDL_Event event;    // Structure of the SDL event, named event

    // Poll untill all events are handled!
    // https://wiki.libsdl.org/SDL2/SDL_PollEvent
    while (SDL_PollEvent(&event) == 1) {
        switch (event.type) {
            // https://libsdl.org/release/SDL-1.2.15/docs/html/sdljoyaxisevent.html
            case SDL_JOYAXISMOTION:
                // assert ensures that when a condition is false, program is terminated
                assert(event.jaxis.axis < caps.NumAxes);
                state.axes[event.jaxis.axis] = event.jaxis.value; // Take the values
                break;

            // https://libsdl.org/release/SDL-1.2.15/docs/html/sdljoybuttonevent.html
            case SDL_JOYBUTTONDOWN: // Somehow put the state of the button when it is released
                
            case SDL_JOYBUTTONUP:
                assert(event.jbutton.button < caps.NumButtons);
                state.buttons[event.jbutton.button] = event.jbutton.state; // Take the values
                break;

            // https://libsdl.org/release/SDL-1.2.15/docs/html/sdljoyhatevent.html
            case SDL_JOYHATMOTION:
                assert(event.jhat.hat < caps.NumHats);
                state.hats[event.jhat.hat] = event.jhat.value;
                break;

            case SDL_JOYBALLMOTION: // do nothing, x52p has no balls. Damn, son.   
                break;

            // Not really necessary, but for educational purpose.    
            case SDL_QUIT:
                printf("Quit the main loop, exits now \n");
                break;    
        }
    } 
}

// Close the SDL device
void Yoke_linux64::UnacqDev() {
    SDL_JoystickClose(theyokes);
    free(state.axes);
    free(state.hats);
    free(state.buttons);
}

///////////////////////////////// AXES /////////////////////////////////
double Yoke_linux64::Steer() {
	if (state.axes[0] > deadzone || state.axes[0] < -deadzone) {
		return state.axes[0] / thrs;
	}
	else {
		return 0.0f;
	}
}

double Yoke_linux64::Nose() {
	if (state.axes[1] > deadzone || state.axes[1] < -deadzone) {
		return -state.axes[1] / thrs;
	}
	else {
		return 0.0f;
	}
}

double Yoke_linux64::BlackClutch() {
    // SDL makes it from -thrs to thrs
	double tmpZ = thrs - state.axes[2]; // change to 0 to thrsZ
    
    if (tmpZ > deadzone) {
        return tmpZ / thrsZ;
    }
    else {
        return 0.0f;
    }
}

double Yoke_linux64::BlueClutch() {
	// SDL makes it from -thrs to thrs
	double tmpZ = thrs - state.axes[3]; // change to 0 to thrsZ
    
    if (tmpZ > deadzone) {
        return tmpZ / thrsZ;
    }
    else {
        return 0.0f;
    }
}

double Yoke_linux64::RedClutch() {
	// SDL makes it from -thrs to thrs
	double tmpZ = thrs - state.axes[4]; // change to 0 to thrsZ
    
    if (tmpZ > deadzone) {
        return tmpZ / thrsZ;
    }
    else {
        return 0.0f;
    }
}

///////////////////////////////// AIMPOV /////////////////////////////////
int Yoke_linux64::povdeg() {
    int tmpA = state.hats[0];

    switch (tmpA) {
        case 0:
            tmpA = -1000;
            break;
        case 1:
            tmpA = 0;
            break;
        case 2:
            tmpA = 90;
            break;
        case 3:
            tmpA = 45;
            break;
        case 4:
            tmpA = 180;
            break;
        case 6:
            tmpA = 135;
            break;
        case 8:
            tmpA = 270;
            break;
        case 9:
            tmpA = 315;
            break;
        case 12:
            tmpA = 225;
            break;                            
    }
    return tmpA;
}

///////////////////////////////// BUTTONS /////////////////////////////////
int Yoke_linux64::IsButtonPressed(int button_id) {
    return state.buttons[button_id];
}
