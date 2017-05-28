/*
	此相机系统，当朝（0,-1,0）方向观察时，
	水平方向的旋转会出现问题，当前无法解决该问题！
*/
#ifndef					__CAMERA_H__
#define					__CAMERA_H__

#include "Common.h"
#include "Singleton.h"

class Camera
{
public:
	enum MoveDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	enum MouseMove
	{
		NONE,
		ANGLE,
		LENGTH
	};

public:
	Camera() = default;

	Camera(GLfloat aspect_, const glm::vec3& eye_ = glm::vec3(0.0, 0.0, 20.0),
		const glm::vec3& center_ = glm::vec3(0.0f ,0.0f, 0.0f),
		const glm::vec3& up_ = glm::vec3(0.0f, 1.0f, 0.0f));

public:
	void resetCamera();

	void bindUniformBuffer(GLuint bindPoint_);

	glm::vec3 getPosition() const;

public:
	void processKeyboard(MoveDirection direction_, GLfloat deltaTime_);

	void processMouseMove(GLfloat xOffset_, GLfloat yOffset_, MouseMove state_, GLboolean constraiPitch_ = true);

	void processMouseScroll(GLfloat yOffset_);

private:
	bool init();

	glm::mat4 getViewMatrix() const;

	glm::mat4 getProjectionMatrix() const;

	void updateCameraVector();

	void updateUniformBuffer();

	void mouseMoveView(GLfloat xOffset_, GLfloat yOffset_, GLboolean constraiPitch_);

	void mouseMoveLength(GLfloat xOffset_, GLfloat yOffset_);


private:
// 	Camera(const Camera&) = delete;
// 
// 	Camera& operator=(const Camera&) = delete;

private:
	GLuint					_UBO;
	GLboolean			_uboCreated;

	glm::vec3				_eye;
	glm::vec3				_center;
	glm::vec3				_up;
	glm::vec3				_front;
	glm::vec3				_right;

	GLfloat					_pitch;
	GLfloat					_yaw;

	GLfloat					_aspect;
	GLfloat					_fovy;

	GLfloat					_moveSpeed;
	GLfloat					_mouseSensitivity;
};

#endif