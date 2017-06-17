#include <iostream>
#include <windows.h>

#include "Application.h"
#include "Common.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Timer.h"

Application::Application(int width_, int height_,
	const char* winName_ /* = "����" */)
{
	if (!initWindow(width_, height_, winName_))
	{
		std::cout << "���ڳ�ʼ��ʧ�ܣ�" << std::endl;
		return;
	}
	glCheckError();
	initGlew();
	glCheckError();
	initRender();	//������Ⱦ״̬������glClearColor��
	glCheckError();
	setFPS(30);
}

void Application::exec()
{
	glEnable(GL_DEPTH_TEST);	//������Ȳ���
	glEnable(GL_STENCIL_TEST);	//����ģ�����
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	while (!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();
 		if (InputManager::s_isPause == GL_FALSE)
		{
		 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			static Timer timer; 
			processInput(timer.calcInvertal());
			draw();
		}
		glfwSwapBuffers(_window);
		sleep();
	}
	exit();
}

void Application::setFPS(unsigned int fps_)
{
	this->_fps = fps_;
}

glm::vec3 Application::getCameraPosition() const
{
	return _camera.getPosition();
}

bool Application::initWindow(int width_, int height_,
	const char* winName_ /* = "����" */)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(width_, height_, winName_, 0, 0);
	if (_window == nullptr)
	{
		exit();
		return false;
	}
	glfwMakeContextCurrent(_window);
	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(_window, keyCallback);
	glfwSetCursorPosCallback(_window, mouseMoveCallback);
	glfwSetScrollCallback(_window, mouseScrollCallback);
	glfwSetMouseButtonCallback(_window, mouseButtonCallback);

	//��������ڴ����м�
	glfwSetCursorPos(_window, width_ / 2.0f, height_ / 2.0f);

	return true;
}

bool Application::initGlew()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initalize GLEW" << std::endl;
		return false;
	}
	return true;
}

bool Application::initRender()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);

	ResourceManager::getInstance().initResource();

	glCheckError();

	_camera = Camera((float)width / (float)height);
	_camera.bindUniformBuffer(CAMERA_UNIFORM_BIND_POINT);
	
	return true;
}

void Application::exit()
{
	glfwTerminate();
}

void Application::sleep()
{
	static Timer timer;
	double sleepTime = 1.0 / _fps - timer.calcInvertal();
	sleepTime = sleepTime > 0 ? sleepTime : 0;
	Sleep(sleepTime * 1000);
}

void Application::processInput(GLfloat dt)
{
	if (InputManager::getInstance().isKeyPressed(GLFW_KEY_W))
	{
		_camera.processKeyboard(Camera::FORWARD, dt);
	}

	if (InputManager::getInstance().isKeyPressed(GLFW_KEY_S))
	{
		_camera.processKeyboard(Camera::BACKWARD, dt);
	}

	if (InputManager::getInstance().isKeyPressed(GLFW_KEY_A))
	{
		_camera.processKeyboard(Camera::LEFT, dt);
	}

	if (InputManager::getInstance().isKeyPressed(GLFW_KEY_D))
	{
		_camera.processKeyboard(Camera::RIGHT, dt);
	}

	_camera.processMouseMove(InputManager::getInstance().s_deltaX,
		InputManager::getInstance().s_deltaY, InputManager::s_mouseMoveState);
	_camera.processMouseScroll(InputManager::getInstance().s_mouseScroll);

	InputManager::getInstance().clearMouseScroll();
	InputManager::getInstance().clearMouseMove();
}

void Application::keyCallback(GLFWwindow* window_, int key_, int scancode_,
	int action_, int mode_)
{
	//���û�����ESC��������window���ڵ�WindowShouldClose����Ϊtrue
	//�ر�Ӧ�ó���
	if (key_ == GLFW_KEY_ESCAPE && action_ == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_, GL_TRUE);
	}

	//���û�����SPACE��ʱ����ͣ��Ⱦ���̣��ٴΰ��£�������Ⱦ
	if (key_ == GLFW_KEY_SPACE && action_ == GLFW_PRESS)
	{
		InputManager::pressSpaceKey();
	}

	InputManager::getInstance().processKeyboard(key_, action_);
}

void Application::mouseMoveCallback(GLFWwindow* window_, double xPos_,
	double yPos_)
{
	InputManager::getInstance().processMouseMove(xPos_, yPos_);
}

void Application::mouseScrollCallback(GLFWwindow* window_, double xOffset_,
	double yOffset_)
{
	InputManager::processMouseScroll(yOffset_);
}

void Application::mouseButtonCallback(GLFWwindow* window, int button,
	int action, int mods)
{
	if (action != GLFW_PRESS)
	{
		InputManager::s_mouseMoveState = Camera::NONE;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		InputManager::s_mouseMoveState = Camera::ANGLE;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		InputManager::s_mouseMoveState = Camera::LENGTH;
	}
}