#include "InputManager.h"

GLboolean InputManager::s_keyPressed[1024] = { GL_FALSE };

GLfloat		InputManager::s_mouseX = 0.0f;
GLfloat		InputManager::s_mouseY = 0.0f;
GLfloat		InputManager::s_deltaX	   = 0.0f;;	
GLfloat		InputManager::s_deltaY    = 0.0f;;
GLfloat		InputManager::s_mouseScroll = 0.0f;
Camera::MouseMove InputManager::s_mouseMoveState = Camera::NONE;
GLboolean InputManager::s_isPause = GL_FALSE;

GLboolean InputManager::isKeyPressed(GLint key_)
{
	return s_keyPressed[key_];
}

void InputManager::pressSpaceKey()
{
	s_isPause = !s_isPause;
}

void InputManager::processKeyboard(GLint key_, GLint action_)
{
	if (key_ >= 0 && key_ <= 1024)
	{
		if (action_ == GLFW_PRESS)
		{
			s_keyPressed[key_] = true;
		}
		else if (action_ == GLFW_RELEASE)
		{
			s_keyPressed[key_] = false;
		}
	}
}

void InputManager::processMouseMove(GLint x_, GLint y_)
{
	static bool firstMove = true;
	if (firstMove)
	{
		s_mouseX = x_;
		s_mouseY = y_;
		firstMove = false;
	}
	s_deltaX	= x_ - s_mouseX;
	s_deltaY	= y_ - s_mouseY;

	s_mouseX = x_;
	s_mouseY = y_;
}

void InputManager::processMouseScroll(GLfloat yOffset_)
{
	s_mouseScroll = yOffset_;
}

void InputManager::clearMouseScroll()
{
	s_mouseScroll = 0.0f;
}

void InputManager::clearMouseMove()
{
	s_deltaX = 0.0f;
	s_deltaY = 0.0f;
}