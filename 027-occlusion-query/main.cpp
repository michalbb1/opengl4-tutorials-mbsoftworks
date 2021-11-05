// STL
#include <iostream>

// Project
#include "027-occlusion-query.h"

/**
 * Application entry point function.
 */
int main()
{
	const std::string& windowTitle = "027.) Occlusion Query - Tutorial by Michal Bubnar (www.mbsoftworks.sk)";
    const int majorVersion = 4;
    const int minorVersion = 4;

#ifdef _WIN32
    const auto showFullscreen = MessageBox(nullptr, "Would you like to run in fullscreen mode?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES;
#else
    char answer;
    std::cout << "Would you like to run in fullscreen mode? (y/n): ";
    std::cin >> answer;
    bool showFullscreen = tolower(answer) == 'y';
#endif
	
	OpenGLWindow027 window;
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