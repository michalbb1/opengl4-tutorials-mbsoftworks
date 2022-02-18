// STL
#include <iostream>

// Project
#include "001-creating-opengl4-window.h"

/**
 * Application entry point function.
 * Uses MessageBox function on Windows systems to ask about fullscreen mode and console query on Unix systems.
 */
int main()
{
	const std::string& windowTitle = "001.) Creating OpenGL4 Window - Tutorial by Michal Bubnar (www.mbsoftworks.sk)";
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

	opengl4_mbsoftworks::tutorial001::OpenGLWindow001 window;
	if (!window.createOpenGLWindow(windowTitle, majorVersion, minorVersion, showFullscreen))
	{
		std::cout << "Failed to create window with OpenGL context " << majorVersion << "." << minorVersion << "! Shutting down..." << std::endl;
		return 1;
	}

	window.runApp();

	return 0;
}