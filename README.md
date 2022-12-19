# FlightYokeSystem
Logitech/Saitek Flight Yoke System joystick interface (C++, Simulink), using DirectX API (DirectInput and Direct Output). This requires DirectX.
Developed on the basis of the samples shown in https://github.com/walbourn/directx-sdk-samples/tree/main/DirectInput/Joystick.

Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812. This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

Necessary files to be in your folder (where your .slx present, or the referenced path):
  1. YokeSystem.h the header file
  2. YokeSystem.cpp the function definition file: this file
  3. Your .cpp impelementation file (put it in Source Files)

**HOW TO COMPILE IN VISUAL STUDIO** <br/>
Include the YokeSystem.h in the header files (project tree) and in your folder.
When something goes wrong, you can manually set everything in the project properties.
1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
   (Find the folder in your DirectX SDK installation directory).
2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
   DirectX SDK (find it in your DirectX SDK installation directory).
3. In the Linker -> Input -> Additional Dependencies, write dinput8.lib; dxguid.lib, add.

**HOW TO COMPILE (FOR MATLAB SIMULINK)**
1. You need C++ compiler available in your system. 
2. Be sure to set the compiler appropriately for C++, use "mex -setup" and choose accordingly. 
3. Compile in Matlab with "mex YokeSystem_SFun.cpp". 
4. This gives you a mexw64 file (Matlab executable file) in the folder.

Open the Simulink file YokeSystem.slx and see more.
You can read more detailed information in each of the files here.
