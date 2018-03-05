#include <iostream>

#include "../common_classes/OpenGLWindow.h"

/**
  Application entry point function. This is Windows version and uses Windows-specific MessageBox call.
*/

int main()
{
	const std::string& windowTitle = "001.) Creating OpenGL4 Window";
	int majorVersion = 4;
	int minorVersion = 4;
	bool showFullscreen = MessageBox(NULL, "Would you like to run in fullscreen mode?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES;

	OpenGLWindow window;
	if (!window.createOpenGLWindow(windowTitle, majorVersion, minorVersion, showFullscreen))
	{
		printf("Failed to create window with OpenGL context %d.%d! Shutting down...", majorVersion, minorVersion);
		return 1;
	}

	window.runApp();

	return 0;
}