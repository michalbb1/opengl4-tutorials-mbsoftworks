#include "OpenGLWindow.h"

std::map<GLFWwindow*, OpenGLWindow*> OpenGLWindow::_windows;

OpenGLWindow::OpenGLWindow()
{
	for (bool& kwp : _keyWasPressed)
	{
		kwp = false;
	}
}

bool OpenGLWindow::createOpenGLWindow(const std::string& windowTitle, int majorVersion, int minorVersion, bool showFullscreen)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	const auto primaryMonitor = glfwGetPrimaryMonitor();
	const auto videoMode = glfwGetVideoMode(primaryMonitor);

	_window = glfwCreateWindow(videoMode->width, videoMode->height, windowTitle.c_str(), showFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (_window == nullptr)
	{
		return false;
	}

	glfwMakeContextCurrent(_window);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSetWindowSizeCallback(_window, onWindowSizeChangedStatic);

	_windows[_window] = this;

	return true;
}

bool OpenGLWindow::keyPressed(int keyCode) const
{
	return glfwGetKey(_window, keyCode) == GLFW_PRESS;
}

bool OpenGLWindow::keyPressedOnce(int keyCode)
{
	bool result = false;
	if (keyPressed(keyCode))
	{
		if (!_keyWasPressed[keyCode])
		{
			result = true;
		}
		
		_keyWasPressed[keyCode] = true;
	}
	else
	{
		_keyWasPressed[keyCode] = false;
	}

	return result;
}

void OpenGLWindow::runApp()
{
	initializeScene();
	
	while (glfwWindowShouldClose(_window) == false)
	{
		renderScene();

		glfwSwapBuffers(_window);
		glfwPollEvents();
		handleInput();
	}

	releaseScene();

	glfwDestroyWindow(_window);
	_windows.erase(_windows.find(_window));
	
	if (_windows.size() == 0)
	{
		glfwTerminate();
	}
}

GLFWwindow* OpenGLWindow::getWindow() const
{
	return _window;
}

void OpenGLWindow::closeWindow(bool hasErrorOccured)
{
	glfwSetWindowShouldClose(_window, true);
	_hasErrorOccured = hasErrorOccured;
}

bool OpenGLWindow::hasErrorOccured() const
{
	return _hasErrorOccured;
}

void OpenGLWindow::onWindowSizeChangedStatic(GLFWwindow* window, int width, int height)
{
	if (_windows.count(window) != 0)
	{
		_windows[window]->onWindowSizeChanged(window, width, height);
	}
}
