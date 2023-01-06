// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Header file, external dependency file.
// Saitek/Logitech Flight Yoke System.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// B.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. YokeSystem.h the header file: this file
//			2. YokeSystem.cpp the function definition file
// ---------------------------------------------------------------------------------------------------------- //


#ifndef YOKESYSTEM_H
#define YOKESYSTEM_H

#define DIRECTINPUT_VERSION 0x0800	// DirectX version, MUST!
#include <dinput.h>		// DirectInput API header
#include <stdio.h>		// For std I/O namespace
#include <iostream>		// For std I/0 stream
#include <Windows.h>	// For ZeroMemory function

#pragma comment(lib, "dinput8.lib")	// The necessary libraries for DirectInput API
#pragma comment(lib, "dxguid.lib")

HINSTANCE hInstance = GetModuleHandle(NULL); // Instance of the window, null is fine

// Define the struct for multiple joysticks
struct Joysticks
{
	unsigned int deviceCount;
	IDirectInputDevice8** Yoke_devs;	// Pointer to the interface of device, name it Yoke_devs
	IDirectInput8* Yoke_inps;			// Pointer to the interface of input, name the pointer as Yoke_inps
};

// Define the required values for normalization of the axes
const double thrs = 32767.0f, thrsZ = 65535.0f;
const int deadzone = 3500;	// Deadzone to prevent jumps due to light touch at the origins

class YokeSystem {
public:						// Access specifier, public = can be accessed and modified outside the class
	// Class constructors!
	YokeSystem();			// Default constructor 
	YokeSystem(int id);		// Constructor, accepts arguments, will be defined outside the class via Class::Class( args )

	// Class methods for DirectInput!
	Joysticks InitDev();	// Methods (functions belong to a class, defined outside the class via void Class::Class( args ) { }
	DIDEVCAPS GetCaps();
	DIJOYSTATE2 GetState();
	int GetButtonNum();
	void UnacqDev();

	// Class methods for state normalization!
	double Steer();	// void has no return type, double returns a double type, bool returns binary 1 or 0 (T/F)
	double Nose();
	double BlackClutch();
	double BlueClutch();
	double RedClutch();
	int povdeg();

	// Class methods for buttons
	int IsButtonPressed(int button_id);

	// Class methods for debugging ID and device
	int IsDevConnected();
	int GetDevID();

private:
	// Class important variables! In private for safety! Comment out the above //private: for debugging!
	DIDEVCAPS caps;			// DIDEVCAPS structure of DirectInput https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416607(v=vs.85)
	DIJOYSTATE2 state;		// DJOYSTATE structure of DirectInput https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416627(v=vs.85)
	Joysticks theyokes;
	int joystick_id;
};

// DirectOuput LED IDs
//DWORD LED_FIRE = 0;
//DWORD LED_FIRE_A_RED = 1;
//DWORD LED_FIRE_A_GREEN = 2;
//DWORD LED_FIRE_B_RED = 3;
//DWORD LED_FIRE_B_GREEN = 4;
//DWORD LED_FIRE_D_RED = 5;
//DWORD LED_FIRE_D_GREEN = 6;
//DWORD LED_FIRE_E_RED = 7;
//DWORD LED_FIRE_E_GREEN = 8;
//DWORD LED_TOGGLE_1_2_RED = 9;
//DWORD LED_TOGGLE_1_2_GREEN = 10;
//DWORD LED_TOGGLE_3_4_RED = 11;
//DWORD LED_TOGGLE_3_4_GREEN = 12;
//DWORD LED_TOGGLE_5_6_RED = 13;
//DWORD LED_TOGGLE_5_6_GREEN = 14;
//DWORD LED_POV_2_RED = 15;
//DWORD LED_POV_2_GREEN = 16;
//DWORD LED_CLUTCH_RED = 17;
//DWORD LED_CLUTCH_GREEN = 18;
//DWORD LED_THROTTLE = 19;

#endif