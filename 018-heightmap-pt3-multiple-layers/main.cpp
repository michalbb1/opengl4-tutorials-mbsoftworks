#include <iostream>

#include "../common_classes/OpenGLWindow.h"

/**
Application entry point function. This is Windows version and uses Windows-specific MessageBox call.
*/

int main()
{
	const std::string& windowTitle = "018.) Heightmap Pt.3 - Multiple Layers - Tutorial by Michal Bubnar (www.mbsoftworks.sk)";
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
	
	OpenGLWindow window;
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