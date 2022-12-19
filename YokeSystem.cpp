// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Functions definitions file. Requires YokeSystem.h header!
// This file is not a main file (implementation), you need to make your own implementation file!
// Nevertheless, a sample is provided, namely TESTYokeSystem.cpp.
// Saitek/Logitech Flight Yoke System.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// B.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. YokeSystem.h the header file
//			2. YokeSystem.cpp the function definition file: this file
//			3. Your .cpp impelementation file (put it in Source Files)

// HOW TO COMPILE IN VISUAL STUDIO
// Include the YokeSystem.h in the header files (project tree) and in your folder.
// When something goes wrong, you can manually set everything in the project properties.
//	1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
//	   (Find the folder in your DirectX SDK installation directory).
//  2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
//	   DirectX SDK (find it in your DirectX SDK installation directory).
//	3. In the Linker -> Input -> Additional Dependencies, write dinput8.lib; dxguid.lib, add.
// ---------------------------------------------------------------------------------------------------------- //


#include "YokeSystem.h"				// External dependency file, header file

YokeSystem::YokeSystem() {			// To instantiate a Class object, default
	joystick_id = 0;	// Default set joystick ID as 0
	InitDev();			// Initialize DirectInput
	GetCaps();			// Get caps
}

YokeSystem::YokeSystem(int id) {		// To instantiate a Class object
	joystick_id = id;	// Set manual joystick ID 
	InitDev();			// Initialize DirectInput
	GetCaps();			// Get caps
}

// Callback for EnumDevices method (function in a Class) https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416622(v=vs.85)
BOOL CALLBACK DirectEnumCB(LPCDIDEVICEINSTANCE instance, LPVOID context) {
	Joysticks* joysticks = (Joysticks*)context; // Assign a pointer for something with Joysticks struct pointer, name it joysticks

	IDirectInputDevice8* YokeS_d;		// Create a pointer for the interface of selected device, name it YokeS_d

	// Method to create and initialize an instance of a device, obtain a device interface 
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417803(v=vs.85)
	joysticks->Yoke_inps->CreateDevice(instance->guidInstance, &YokeS_d, NULL); // Put it in the pointer of Yoke_inps (input interface)

	// Method to set coop level, BG access: device can be acquired at any time
	// NONEXCLUSIVE: access to device does not interefere with others who are accessing the same device
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)
	YokeS_d->SetCooperativeLevel(GetActiveWindow(), DISCL_BACKGROUND | DISCL_EXCLUSIVE); // Access selected device interface via pointer

	// Method to set the format to a joystick (not keyboard etc.) https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417925(v=vs.85)
	YokeS_d->SetDataFormat(&c_dfDIJoystick2); // Access selected device interface via pointer

	// Method to gain access to the device, AFTER SetDataFormat
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417818(v=vs.85)
	YokeS_d->Acquire(); // Access selected device interface via pointer

	// Update deviceCount by accessing the elements of a struct via pointer 
	joysticks->deviceCount += 1;

	// Since we do assume there will be many devices, then realloc the pointer to pointer (device interface)
	joysticks->Yoke_devs = (IDirectInputDevice8**)realloc(joysticks->Yoke_devs, joysticks->deviceCount * sizeof(IDirectInputDevice8*));

	// Assign the pointer to pointer of the device with that of the selected device interface
	joysticks->Yoke_devs[joysticks->deviceCount - 1] = YokeS_d;

	// To continue enumeration, if you want to stop use DIENUM_STOP
	// return DIENUM_CONTINUE;
	// ONLY ONE DEVICE
	return DIENUM_CONTINUE;
}

// Initialize and create device, do it just once
Joysticks YokeSystem::InitDev() {
	theyokes = { 0 }; // initialization of the struct, namely joysticks with Joysticks struct

	// Creates a DirectInput object https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416756(v=vs.85)
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&theyokes.Yoke_inps, 0);

	// Arrow operator -> allows access elements in a struct via pointer that points to a struct.
	// Similar to dot but dot access elements in a struct directly.
	// Enumerat all devices https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417804(v=vs.85)
	theyokes.Yoke_inps->EnumDevices(DI8DEVCLASS_GAMECTRL, DirectEnumCB, (void*)&theyokes, DIEDFL_ALLDEVICES);

	return theyokes;
}

// Get device capabilities
DIDEVCAPS YokeSystem::GetCaps() {
	// Describes the DirectInput devices capabiliteis, DIDEVCAPS is a structure
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416607(v=vs.85)
	caps = { sizeof(DIDEVCAPS) }; // Allocate memory

	// Get the capabilities of the device object
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417892(v=vs.85)
	theyokes.Yoke_devs[0]->GetCapabilities(&caps); // Put it in the memory address of caps with DIDEVCAPS struct

	return caps;
}

// Get button number from caps
int YokeSystem::GetButtonNum() {
	return button_num = caps.dwButtons;
}

// Get the state from the device, do it every step
DIJOYSTATE2 YokeSystem::GetState() {
	// Method to get the state of the device
	// Puts the state in the memory addres of state with DIJOYSTATE struct returns a DI_OK
	// Must create, set cooperative level, data format, and acquire, in that order
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417897(v=vs.85)
	// ONLY ONE DEVICE
	ZeroMemory(&state, sizeof(DIJOYSTATE2));    //Set Memory to 0
	theyokes.Yoke_devs[joystick_id]->GetDeviceState(sizeof(state), &state);

	return state;
}

// Check device exists or not
int YokeSystem::IsDevConnected() {
	HRESULT hr = theyokes.Yoke_devs[joystick_id]->GetDeviceState(sizeof(state), &state);
	if (hr == DI_OK) {
		return 1;
	}
	else {
		return 0;
	}
}

// Get joystick ID
int YokeSystem::GetDevID() {
	return joystick_id;
}

// Unacquire the device
void YokeSystem::UnacqDev() {
	HRESULT hr;
	hr = theyokes.Yoke_devs[joystick_id]->Unacquire();
	//if (hr == DI_OK) {
	//	std::cout << "Unacquiring success";
	//}
}

////////////////////////////  AXES ///////////////////////////////////////
double YokeSystem::Steer() {
	if (state.lX > thrs + deadzone || state.lX < thrs - deadzone) {
		return (state.lX - thrs) / thrs;
	}
	else {
		return 0.0f;
	}
}

double YokeSystem::Nose() {
	if (state.lY > thrs + deadzone || state.lY < thrs - deadzone) {
		return((thrs - state.lY) / thrs);
	}
	else {
		return 0.0f;
	}
}

double YokeSystem::BlackClutch() {
	if (state.lZ > thrsZ + deadzone || state.lZ < thrsZ - deadzone) {
		return((thrsZ - state.lZ) / thrsZ);
	}
	else {
		return 0.0f;
	}
}

double YokeSystem::BlueClutch() {
	if (state.lRx > thrsZ + deadzone || state.lRx < thrsZ - deadzone) {
		return((thrsZ - state.lRx) / thrsZ);
	}
	else {
		return 0.0f;
	}
}

double YokeSystem::RedClutch() {
	if (state.lRy > thrsZ + deadzone || state.lRy < thrsZ - deadzone) {
		return((thrsZ - state.lRy) / thrsZ);
	}
	else {
		return 0.0f;
	}
}

//////////////////////////// AIMPOV ///////////////////////////////////////
int YokeSystem::povdeg() {
	int pov = state.rgdwPOV[0] / 100;	// Return the aim at every 45 deg.
	if (state.rgdwPOV[0] == -1) {
		pov = -1000;	// When not used, it gives -1000.
	}
	return pov;
}

//////////////////////////// BUTTONS //////////////////////////////////////
int YokeSystem::IsButtonPressed(int button_id) {
	if (state.rgbButtons[button_id]) {
		return 1;	// If button pressed returns 1 as integer
	}
	else {
		return 0;
	}
}