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

public:
	Camera() = default;

	Camera(GLfloat aspect_, const glm::vec3& eye_ = glm::vec3(0.0, 0.0, 20.0),
		const glm::vec3& up_ = glm::vec3(0.0f,1.0f, 0.0f));

public:
	void resetCamera();

	void bindUniformBuffer(GLuint bindPoint_);

	glm::vec3 getPosition() const;

public:
	void processKeyboard(MoveDirection direction_, GLfloat deltaTime_);

	void processMouseMove(GLfloat xOffset_, GLfloat yOffset_, GLboolean constraiPitch_ = true);

	void processMouseScroll(GLfloat yOffset_);

private:
	bool init();

	glm::mat4 getViewMatrix() const;

	glm::mat4 getProjectionMatrix() const;

	void updateCameraVector();

	void updateUniformBuffer();

private:
// 	Camera(const Camera&) = delete;
// 
// 	Camera& operator=(const Camera&) = delete;

private:
	GLuint					_UBO;
	GLboolean			_uboCreated;

	glm::vec3				_initEye;
	glm::vec3				_initUp;

	glm::vec3				_eye;
	//glm::vec3				_center;
	glm::vec3				_worldUp;
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