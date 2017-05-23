#include "Camera.h"

const GLfloat	  YAW					=		-90.0f;
const GLfloat	  PITCH					=		0.0f;
const GLfloat	  SPEED					=		3.0f;
const GLfloat	  SENSITIVITY		=		0.05f;
const GLfloat	  FOVY					=		45.0f;


Camera::Camera(GLfloat aspect_, const glm::vec3& eye_, const glm::vec3& up_)
	:_aspect(aspect_), _initEye(eye_), _initUp(up_)
{
	this->_uboCreated = GL_FALSE;
	init();
}

void Camera::resetCamera()
{
	init();
	glCheckError();
	this->updateUniformBuffer();
	glCheckError();
}

void Camera::processKeyboard(MoveDirection direction_, GLfloat deltaTime_)
{
	GLfloat velocity = this->_moveSpeed * deltaTime_;
	if (direction_ == FORWARD)
	{
		this->_eye -= this->_front * velocity;
	}
	else if (direction_ == BACKWARD)
	{
		this->_eye += this->_front * velocity;
	}
	else if (direction_ == LEFT)
	{
		this->_eye += this->_right * velocity;
	}
	else if (direction_ == RIGHT)
	{
		this->_eye -= this->_right * velocity;
	}
	glCheckError();
	this->updateUniformBuffer();
	glCheckError();
}

void Camera::processMouseMove(GLfloat xOffset_, GLfloat yOffset_,
	GLboolean constraiPitch_ /* = true */)
{
	xOffset_ *= _mouseSensitivity;
	yOffset_ *= _mouseSensitivity;

	this->_yaw	-= xOffset_;
	this->_pitch	+= yOffset_;

	if (constraiPitch_)
	{
		if (this->_pitch > 444.0f)
		{
			this->_pitch = 44.0f;
		}
		else if (this->_pitch < -44.0f)
		{
			this->_pitch = -44.0f;
		}
	}
	glCheckError();
	this->updateCameraVector();
	this->updateUniformBuffer();
	glCheckError();
}

void Camera::processMouseScroll(GLfloat yOffset_)
{
	if (this->_fovy >= 1.0f && this->_fovy <= 75.0f)
	{
		this->_fovy -= yOffset_;
	}
	if (this->_fovy > 75.0f)
	{
		this->_fovy = 75.0f;
	}
	else if (this->_fovy < 1.0f)
	{
		this->_fovy = 1.0f;
	}
	glCheckError();
	this->updateUniformBuffer();
	glCheckError();
}

void Camera::bindUniformBuffer(GLuint bindPoint_)
{
	if (!_uboCreated)
	{
		glGenBuffers(1, &this->_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, this->_UBO);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		_uboCreated = GL_TRUE;
	}
	glCheckError();
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint_, _UBO, 0, 2 * sizeof(glm::mat4));
	updateUniformBuffer();
	glCheckError();
}

glm::vec3 Camera::getPosition() const
{
	return _eye;
}

bool Camera::init()
{
	this->_fovy = FOVY;
	this->_yaw = YAW;
	this->_pitch = PITCH;
	this->_moveSpeed = SPEED;
	this->_mouseSensitivity = SENSITIVITY;
	this->_eye = _initEye;
	this->_worldUp = _initUp;
	this->_front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->updateCameraVector();

	return true;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_eye, _eye + _front, _up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(_fovy, _aspect, 1.0f, 200.0f);
}

void Camera::updateCameraVector()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	this->_front = glm::normalize(front);
	this->_right = glm::normalize(glm::cross(_front, _worldUp));
	this->_up	   = glm::normalize(glm::cross(_right, _front));
}

void Camera::updateUniformBuffer()
{
	glm::mat4 view = getViewMatrix();
	glm::mat4 projection = getProjectionMatrix();
	glCheckError();
	glBindBuffer(GL_UNIFORM_BUFFER, this->_UBO);
	glCheckError();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glCheckError();
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(projection), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glCheckError();
}