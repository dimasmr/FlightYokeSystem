// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Sample of implementation. Requires YokeSystem.h header and YokeSystem.cpp functions definitions files.
// Saitek/Logitech Flight Yoke System.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// C.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. YokeSystem.h the header file
//			2. YokeSystem.cpp the function definition file
//			3. Your .cpp impelementation file: as an example this file (put it in Source Files).

// HOW TO COMPILE IN VISUAL STUDIO
// Include the YokeSystem.h in the header (project tree) and in your folder.
// When something goes wrong, you can manually set everything in the project properties.
//	1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
//	   (Find the folder in your DirectX SDK installation directory).
//  2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
//	   DirectX SDK (find it in your DirectX SDK installation directory).
//	3. In the Linker -> Input -> Additional Dependencies, write dinput8.lib; dxguid.lib, add.
// ---------------------------------------------------------------------------------------------------------- //

#include "YokeSystem.h"				// External dependency file, header file
#include "YokeSystem.cpp"			// Functions definitions

//////////////////////////// TESTS HERE ///////////////////////////////////
// Variable type declaration
int button_id;
double axes[5];
double buttons[23];
double slider;
double povaim;

// Main implementation
int main() {
	YokeSystem controller(0);	// Instantiate and object/instance with ID as argument
	//controller.InitDev();

	while (1) {
		//bool resp = controller.IsDevConnected();	// Debugging section
		//std::cout << resp;
		//sint chkID = controller.GetDevID();
		//std::cout << chkID;
		//std::cout << controller.theyokes.Yoke_devs;	// Should persist

		controller.GetState();	// Get device's state
		controller.GetCaps();	// Get device's capabilities
		int button_num = controller.GetButtonNum();	// Get the button number

		axes[0] = controller.Steer();	// Axes
		axes[1] = controller.Nose();
		axes[2] = controller.BlackClutch();
		axes[3] = controller.BlueClutch();
		axes[4] = controller.RedClutch();

		povaim = controller.povdeg();	// PovAim

		for (int i = 0; i < button_num; ++i) {
			buttons[i] = controller.IsButtonPressed(i);	// Buttons
			if (buttons[i] == 1) {
				switch (i) {
				case 0:
					printf("Steer button %1d (left index). ", i + 1);
					break;
				case 1:
					printf("Steer button %1d (red fire). ", i + 1);
					break;
				case 2:
					printf("Steer button %1d (A1). ", i + 1);
					break;
				case 3:
					printf("Steer button %1d (A2). ", i + 1);
					break;
				case 4:
					printf("Steer button %1d (B1). ", i + 1);
					break;
				case 5:
					printf("Steer button %1d (B2). ", i + 1);
					break;
				case 6:
					printf("Steer button %1d (C1). ", i + 1);
					break;
				case 7:
					printf("Steer button %1d (C2). ", i + 1);
					break;
				case 14:
					printf("Clutch toggle %1d (T1/left up). ", i + 1);
					break;
				case 15:
					printf("Clutch toggle %1d (T2/left down). ", i + 1);
					break;
				case 16:
					printf("Clutch toggle %1d (T3/mid up). ", i + 1);
					break;
				case 17:
					printf("Clutch toggle %1d (T4/mid down). ", i + 1);
					break;
				case 18:
					printf("Clutch toggle %1d (T5/rigth up). ", i + 1);
					break;
				case 19:
					printf("Clutch toggle %1d (T6/right down). ", i + 1);
					break;
				case 20:
					printf("Black Cluctch locked %1d. ", i + 1);
					break;
				case 21:
					printf("Blue Cluctch locked %1d. ", i + 1);
					break;
				case 22:
					printf("Red Cluctch locked %1d. ", i + 1);
					break;
				}
				//printf("Button:%1d ", i);	// Check simultaneously, comment out the above whole switch
			}
		}
		printf("\n ");

		printf("Steer:%4f ", axes[0]);	// Print on screen
		printf("Nose:%4f ", axes[1]);
		printf("BlackCluctch:%4f ", axes[2]);
		printf("BlueClutch:%4f ", axes[3]);
		printf("RedClutch:%4f ", axes[4]);

		printf("AimDeg:%4f ", povaim);

	}
	controller.UnacqDev();	// Unacquire device
	Sleep(15);
}