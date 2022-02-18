// STL
#include <iostream>

// Project
#include "004-entering-third-dimension.h"

/**
 * Application entry point function.
 * Uses MessageBox function on Windows systems to ask about fullscreen mode and console query on Unix systems.
 */
int main()
{
	const std::string& windowTitle = "004.) Entering Third Dimension - Tutorial by Michal Bubnar (www.mbsoftworks.sk)";
	constexpr auto majorVersion = 4;
	constexpr auto minorVersion = 4;

#ifdef _WIN32
	const auto showFullscreen = MessageBox(nullptr, "Would you like to run in fullscreen mode?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES;
#else
	char answer;
	std::cout << "Would you like to run in fullscreen mode? (y/n): ";
	std::cin >> answer;
	const auto showFullscreen = tolower(answer) == 'y';
#endif

	opengl4_mbsoftworks::tutorial004::OpenGLWindow004 window;
	if (!window.createOpenGLWindow(windowTitle, majorVersion, minorVersion, showFullscreen))
	{
		std::cout << "Failed to create window with OpenGL context " << majorVersion << "." << minorVersion << "! Shutting down..." << std::endl;
		return 1;
	}

	window.runApp();

	if (window.hasErrorOccurred())
	{
		std::cout << std::endl << std::endl << "Program could not start because of the above errors! Press ENTER to quit..." << std::endl;
		std::cin.get();
		return 1;
	}

	return 0;
}