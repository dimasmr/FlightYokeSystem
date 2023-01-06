// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2023.
// Sample of implementation. Requires YokeSystem_linux.h header and YokeSystem_linux.cpp functions definitions files.
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

// C.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. YokeSystem_linux.h the header file
//			2. YokeSystem_linux.cpp the function definition file
//			3. Your .cpp impelementation file: as an example this file.

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

#include "YokeSystem_linux.cpp"     // Functions definitions

// Variable type declaration
double locaxis[5];
int povaim;
int buttons[23];

int main() {
    InitSDL();  // Initialize the SDL API interface
    Yoke_linux64 controller(0); // Instantiate an object/instance with ID as argument

    int button_num = controller.GetButtonNum(); // Get the button number

    while (1) {
        controller.GetState();  // Get device's state

        locaxis[0] = controller.Steer(); // Axes
        locaxis[1] = controller.Nose();
        locaxis[2] = controller.BlackClutch();
        locaxis[3] = controller.BlueClutch();
        locaxis[4] = controller.RedClutch();

        povaim = controller.povdeg(); // Jostyick PovAim

        printf("Steer:%4f ", locaxis[0]);	// Print on screen
		printf("Nose:%4f ", locaxis[1]);
		printf("BlackCluctch:%4f ", locaxis[2]);
		printf("BlueClutch:%4f ", locaxis[3]);
		printf("RedClutch:%4f ", locaxis[4]);
        printf("PovJoy:%1d ", povaim);
        
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
                case 8:
                    printf("Steer switch %1d (right index, left). ", i + 1);
                    break;    
                case 9:
                    printf("Steer switch %1d (right index, mid). ", i + 1);
                    break;      
                case 10:
                    printf("Steer switch %1d (right index, right). ", i + 1);
                    break;      
                case 11:
                    printf("FUNCTION %1d. ", i + 1);
                    break;
                case 12:
                    printf("START/STOP %1d. ", i + 1);
                    break;
                case 13:
                    printf("RESET %1d. ", i + 1);
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
    }
    controller.UnacqDev(); // Close device and free memory

}