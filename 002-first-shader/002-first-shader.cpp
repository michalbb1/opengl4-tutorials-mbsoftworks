#include "../common_classes/OpenGLWindow.h"

void OpenGLWindow::initializeScene()
{
	glClearColor(0, 0.5f, 1.0f, 1.0f);
}

void OpenGLWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::releaseScene()
{

}

void OpenGLWindow::handleInput()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE))
		closeWindow();
}

void OpenGLWindow::onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

