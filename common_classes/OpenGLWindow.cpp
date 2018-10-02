#include "OpenGLWindow.h"

#include <glm/gtc/matrix_transform.hpp>

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
	glfwMaximizeWindow(_window);

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
	recalculateProjectionMatrix();
	initializeScene();
	
	// Update time at the beginning, so that calculations are correct
	_lastFrameTime = _lastFrameTimeFPS = glfwGetTime();
	
	while (glfwWindowShouldClose(_window) == 0)
	{
		updateDeltaTimeAndFPS();
		renderScene();

		glfwSwapBuffers(_window);
		glfwPollEvents();
		handleInput();
	}

	releaseScene();

	glfwDestroyWindow(_window);
	_windows.erase(_windows.find(_window));
	
	if (_windows.empty())
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

glm::mat4 OpenGLWindow::getProjectionMatrix() const
{
	return _projectionMatrix;
}

float OpenGLWindow::sof(float value) const
{
	return value * float(_timeDelta);
}

double OpenGLWindow::sof(double value) const
{
	return value * _timeDelta;
}

double OpenGLWindow::getTimeDelta() const
{
	return _timeDelta;
}

int OpenGLWindow::getFPS() const
{
	return _FPS;
}

void OpenGLWindow::recalculateProjectionMatrix()
{
	int width, height;
	glfwGetWindowSize(getWindow(), &width, &height);
	_projectionMatrix = glm::perspective(45.0f, float(width) / float(height), 0.5f, 1000.0f);
}

void OpenGLWindow::updateDeltaTimeAndFPS()
{
	const auto currentTime = glfwGetTime();
	_timeDelta = currentTime - _lastFrameTime;
	_lastFrameTime = currentTime;
	_nextFPS++;

	if(currentTime - _lastFrameTimeFPS > 1.0)
	{
		_lastFrameTimeFPS = currentTime;
		_FPS = _nextFPS;
		_nextFPS = 0;
	}
}

void OpenGLWindow::onWindowSizeChangedStatic(GLFWwindow* window, int width, int height)
{
	if (_windows.count(window) != 0)
	{
		_windows[window]->recalculateProjectionMatrix();
		_windows[window]->onWindowSizeChanged(window, width, height);
	}
}
