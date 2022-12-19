// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// C MEX S-Function for Simulink with NO inputs.
// Saitek/Logitech Flight Yoke System.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// B.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. YokeSystem.h the header file
//			2. YokeSystem.cpp the function definition file
//			3. This file

// HOW TO COMPILE
// You need C++ compiler available in your system.
// Be sure to set the compiler appropriately for C++, use "mex -setup" and choose accordingly.
// Compile in Matlab with "mex YokeSystem_SFun.cpp".
// This gives you a mexw64 file (Matlab executable file) in the folder.
// ---------------------------------------------------------------------------------------------------------- //


#define S_FUNCTION_NAME  YokeSystem_SFun	// define S-Function Name
#define S_FUNCTION_LEVEL 2					// define S-Function Level

#include "simstruc.h"		// For Simulink S-Function
#include "YokeSystem.cpp"	// Functions definitions

// Check parameters, hmm still needs checking
#define MDL_CHECK_PARAMETERS
#if defined (MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)

static void mdlCheckParameters(SimStruct* S) {
	size_t NrParameters = mxGetNumberOfElements(ssGetSFcnParam(S, 0)); // Take number of element in the SFun param
	real_T* joyid_param = (real_T*)mxGetPr(ssGetSFcnParam(S, 0)); // Take pointer of the SFun param, must be double, name it joystick_id

	const char* msg = NULL; // allocate pointer for message of char with null/zeros

	if (NrParameters != 1) { // If people enters array, check!
		msg = "Put only one joystick ID, not an array.";
	}
	else { // If people joystick ID not 0, put error!
		if (joyid_param[0] != 0) {
			msg = "PLEASE USE ID 0, SORRY! WILL FIX THIS WHEN IT IS POSSIBLE";
		}
	}
	if (msg != NULL) { // Give error if and break Simulink if either message is coming
		ssSetErrorStatus(S, msg);
		return;
	}
}
#endif 

// Initialize the S-Function block parameter
static void mdlInitializeSizes(SimStruct* S) {
	ssSetNumSFcnParams(S, 1);	// Expect a parameter: joystick ID, default is 0 !!!
	if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
		mdlCheckParameters(S);	// Run the check, if msg is coming, then break Simulink
		if (ssGetErrorStatus(S) != NULL) {
			return;
		}
	}
	else { // If the number of parameters does not match, break Simulink
		return;
	}

	ssSetNumContStates(S, 0);	// No continuous states
	ssSetNumDiscStates(S, 0);	// No discrete states

	if (!ssSetNumInputPorts(S, 0)) { // No input ports
		return; // Break Simulink if there are input ports
	}

	if (!ssSetNumOutputPorts(S, 3)) { // Four outputs: axes, slider, pov, button
		return; // Break Simulink if the number of output ports is not correct
	}

	ssSetOutputPortWidth(S, 0, 5);	// 1st port: Five axes: steer, nose, and three clutches
	ssSetOutputPortWidth(S, 1, 1);	// 3rd port: One PovAim
	ssSetOutputPortWidth(S, 2, 23);	// 4th port: Twenty three buttons

	ssSetNumSampleTimes(S, -1);	// If sample time is inherited, use -1
	ssSetNumPWork(S, 1);		// Set pointers for persistent objects!
	ssSetNumModes(S, 0);
	ssSetNumNonsampledZCs(S, 0);

	ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT); // Misc for save/restore the Simulink
}

// Initialize the S-Function block parameter: sample time
static void mdlInitializeSampleTimes(SimStruct* S) {
	ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);	// Set to inherit the simulink
	ssSetOffsetTime(S, 0, 0.0);	// No offset
}

// Callback to initialize initial state just once! Make persist the DirectInput object
#define MDL_START 
#if defined(MDL_START) 
static void mdlStart(SimStruct* S) {
	// Get the S-Function parameter, here is the joystick ID
	real_T* joyid_param = (real_T*)mxGetPr(ssGetSFcnParam(S, 0));
	// int joyid_chk = int(joyid_param[0]); // For debugging
	// std::cout << joyid_chk;

	// Check and give error to make sure
	if (joyid_param[0] != 0) {
		ssSetErrorStatus(S, "PLEASE USE ID 0, SORRY! WILL FIX THIS WHEN IT IS POSSIBLE");
	}

	// Use PWork (pointer that points to a vector of pointers)
	// to make DirectInput object persist.
	// Create the object YokeSystem with joystick_id, default is 0!!
	void** PWork = ssGetPWork(S);
	PWork[0] = (void*) new YokeSystem(int(joyid_param[0]));	// allocate memory with new
}
#endif

// Update the output: the state of the Joystick
static void mdlOutputs(SimStruct* S, int_T tid) {
	// Get the output of SFun to be used in Simulink, everything is double
	// Convert to the appropriate data type to a pointer
	real_T* axes = (real_T*)ssGetOutputPortRealSignal(S, 0);
	real_T* povaim = (real_T*)ssGetOutputPortRealSignal(S, 1);
	real_T* buttons = (real_T*)ssGetOutputPortRealSignal(S, 2);

	// Take the pointer to the persistent DirectInput object from the pointer vectors, name it c.
	YokeSystem* c = (YokeSystem*)ssGetPWork(S)[0];

	// Update the state by calling the method in the object
	c->GetState();

	// DEBUGGING, MAKE PUBLIC ALL CLASS VARIABLES
	//std::cout << c->thejoys.Yoke_devs
;	// Print out the DirecInput object, should persist each time step
	//std::cout << c->joystick_id;			// For debuggin please always make public the class variables in the YokeSystem.h
	//std::cout << &c->state.lX;
	//std::cout << &c->state;
	//bool rsp = c->IsDevConnected();
	//std::cout << rsp;
	//int chkID = c->GetDevID();
	//std::cout << chkID;

	// Get number of buttons by calling the method in the object
	int butt_no = c->GetButtonNum();

	// Normalize the values
	axes[0] = c->Steer();	// Axes
	axes[1] = c->Nose();
	axes[2] = c->BlackClutch();
	axes[3] = c->BlueClutch();
	axes[4] = c->RedClutch();

	povaim[0] = c->povdeg();	// POV Aim

	// Take the button states, if true, it returns 1
	for (int i = 0; i < 23; ++i) {
		buttons[i] = c->IsButtonPressed(i);
		if (buttons[i] == 1) {
			//printf("Buttons: %d, ", i);
		}
	}
}

// Unacquire the DirectInput objct and free the memory that we allocate to make persistent object
static void mdlTerminate(SimStruct* S) {
	YokeSystem* c = (YokeSystem*)ssGetPWork(S)[0];	// Take the pointer to persistent object
	c->UnacqDev();	// Unacquire
	delete c;		// Free memory
}

#ifdef  MATLAB_MEX_FILE    // Is this file being compiled as a MEX-file?
#include "simulink.c"      // MEX-file interface mechanism
#else
#include "cg_sfun.h"       // Code generation registration function 
#endif