#ifndef					__INPUT_MANAGER_H__
#define					__INPUT_MANAGER_H__

#include "Common.h"
#include "Singleton.h"

class Application;

class InputManager : public Singleton < InputManager >
{
	friend class Application;
public:
	GLboolean isKeyPressed(GLint key_);

	static void processKeyboard(GLint key_, GLint action_);

	static void processMouseMove(GLfloat x_, GLint y_);

	static void processMouseScroll(GLfloat yOffset_);

	static void clearMouseScroll();

	static void clearMouseMove();

private:
	static GLboolean	s_keyPressed[1024];

	static GLfloat			s_mouseX;
	static GLfloat			s_mouseY;
	static GLfloat			s_deltaX;
	static GLfloat			s_deltaY;
	static GLfloat			s_mouseScroll;
};

#endif