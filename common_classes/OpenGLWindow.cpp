// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
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

    if (!showFullscreen)
    {
        glfwMaximizeWindow(_window);
        // After calling glfwMaximizeWindow, the onWindowSizeChanged somehow does not get called
        // Therefore I call it manually
        int width, height;
        glfwGetWindowSize(_window, &width, &height);
        onWindowSizeChanged(width, height);
    }

    glfwSetScrollCallback(_window, onMouseWheelScrollStatic);
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
    setVerticalSynchronization(true);
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

glm::mat4 OpenGLWindow::getOrthoProjectionMatrix() const
{
    return _orthoMatrix;
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

void OpenGLWindow::setVerticalSynchronization(bool enable)
{
    glfwSwapInterval(enable ? 1 : 0);
    _isVerticalSynchronizationEnabled = enable;
}

bool OpenGLWindow::isVerticalSynchronizationEnabled() const
{
    return _isVerticalSynchronizationEnabled;
}

OpenGLWindow* OpenGLWindow::getDefaultWindow()
{
    return _windows.size() == 0 ? nullptr : (*_windows.begin()).second;
}

void OpenGLWindow::onWindowSizeChanged(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGLWindow::recalculateProjectionMatrix()
{
    int width, height;
    glfwGetWindowSize(getWindow(), &width, &height);
    _projectionMatrix = glm::perspective(45.0f, float(width) / float(height), 0.5f, 1500.0f);
    _orthoMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
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
        _windows[window]->onWindowSizeChanged(width, height);
    }
}

void OpenGLWindow::onMouseWheelScrollStatic(GLFWwindow* window, double scrollOffsetX, double scrollOffsetY)
{
    if (_windows.count(window) != 0)
    {
        _windows[window]->recalculateProjectionMatrix();
        _windows[window]->onMouseWheelScroll(scrollOffsetX, scrollOffsetY);
    }
}
