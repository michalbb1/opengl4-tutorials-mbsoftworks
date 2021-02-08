// STL
#include <iostream>

// Project
#include "026-camera-pt3-orbit-camera.h"

/**
* Application entry point function.
*/
int main()
{
	const std::string& windowTitle = "026.) Camera pt.3 - Orbit Camera - Tutorial by Michal Bubnar (www.mbsoftworks.sk)";
	int majorVersion = 4;
	int minorVersion = 4;

#ifdef _WIN32
    bool showFullscreen = MessageBox(nullptr, "Would you like to run in fullscreen mode?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES;
#else
    char answer;
    std::cout << "Would you like to run in fullscreen mode? (y/n): ";
    std::cin >> answer;
    bool showFullscreen = tolower(answer) == 'y';
#endif
	
	OpenGLWindow026 window;
	if (!window.createOpenGLWindow(windowTitle, majorVersion, minorVersion, showFullscreen))
	{
		printf("Failed to create window with OpenGL context %d.%d! Shutting down...\n", majorVersion, minorVersion);
		return 1;
	}

	window.runApp();

	if (window.hasErrorOccured())
	{
		std::cout << std::endl << std::endl << "Program could not start because of the above errors! Press ENTER to quit..." << std::endl;
		std::cin.get();
		return 1;
	}

	return 0;
}