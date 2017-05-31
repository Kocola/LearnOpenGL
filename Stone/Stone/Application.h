#ifndef					__APPLICATION_H__
#define					__APPLICATION_H__

#include "Camera.h"

class GLFWwindow;

class Application
{
public:
	Application(int width_, int height_, const char* winName_ = "´°¿Ú");

	virtual ~Application()
	{
	}

public:
	void exec();

	void setFPS(unsigned int fps_);

protected:
	virtual void draw() = 0;

	glm::vec3 getCameraPosition() const;

private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

private:
	bool initWindow(int width_, int height_, const char* winName = "´°¿Ú");

	bool initGlew();

	bool initRender();

	void exit();

	void sleep();

	void processInput(GLfloat dt);

private:
	static void keyCallback(GLFWwindow* window_, int key_, int scancode_,
		int action_, int mode_);

	static void mouseMoveCallback(GLFWwindow* window_, double xPos_,
		double yPos_);

	static void mouseScrollCallback(GLFWwindow* window_, double xOffset_,
		double yOffset_);

	static void mouseButtonCallback(GLFWwindow* window, int button,
		int action, int mods);

private:
	GLFWwindow*		_window;
	unsigned int			_fps;
	Camera					_camera;
};

#endif