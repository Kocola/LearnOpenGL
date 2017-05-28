#include "Camera.h"
#include "Utility.h"

const GLfloat	  SPEED					=		3.0f;
const GLfloat	  SENSITIVITY		=		0.05f;
const GLfloat	  FOVY					=		45.0f;

Camera::Camera(GLfloat aspect_, const glm::vec3& eye_ /* = glm::vec3(0.0 , 0.0, 10.0)*/, 
	const glm::vec3& center_ /* = glm::vec3(0.0f ,0.0f, 0.0f)*/,
	const glm::vec3& up_ /* = glm::vec3(0.0f ,1.0f, 0.0f)*/)
	:_aspect(aspect_), _eye(eye_), _center(center_)
{
	this->_fovy = FOVY;

	this->_front = glm::normalize(_center - _eye);
	this->_up = glm::normalize(up_);
	this->_right = glm::cross(_front, _up);

	this->_moveSpeed = SPEED;
	this->_mouseSensitivity = SENSITIVITY;

	this->_uboCreated = GL_FALSE;
	init();
}

void Camera::resetCamera()
{
	init();
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
		this->_eye -= this->_right * velocity;
	}
	else if (direction_ == RIGHT)
	{
		this->_eye += this->_right * velocity;
	}
	this->updateUniformBuffer();
	glCheckError();
}

void Camera::processMouseMove(GLfloat xOffset_, GLfloat yOffset_,
	MouseMove state_,
	GLboolean constraiPitch_ /* = true */)
{
	xOffset_ *= _mouseSensitivity;
	yOffset_ *= _mouseSensitivity;

	if (state_ == ANGLE)
	{
		mouseMoveView(xOffset_, yOffset_, constraiPitch_);
	}
	else if (state_ == LENGTH)
	{
		mouseMoveLength(xOffset_, yOffset_);
	}
	
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
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint_, _UBO, 0, 2 * sizeof(glm::mat4));
	updateUniformBuffer();

	glCheckError();
}

glm::vec3 Camera::getPosition() const
{
	return this->_eye;
}

bool Camera::init()
{
	glm::vec3 curYaw(_front.x, 0.0f, _front.z);
	if (std::abs(glm::length(curYaw)) > std::numeric_limits<float>::epsilon())
	{
		curYaw = glm::normalize(curYaw);
	}

	/* 计算水平旋转角 */
	/* z为垂直轴，方向向下；x为水平轴，方向向右*/
	if (curYaw.z > 0.0f)	// 下半圆
	{
		if (curYaw.x >= 0.0f)	//右下半圆
		{
			_yaw = 360.0f - glm::degrees(glm::asin(curYaw.z));
		}
		else   //左下半圆
		{
			_yaw = 180.0f + glm::degrees(glm::asin(curYaw.z));
		}
	}
	else    //上半圆
	{
		if (curYaw.x >= 0.0f)	//右上半圆
		{
			_yaw = glm::degrees(glm::asin(-curYaw.z));
		}
		else    //左上半圆
		{
			_yaw = 90.0f + glm::degrees(glm::asin(-curYaw.z));
		}
	}

	/* 计算垂直旋转角 */
	_pitch = -glm::degrees(glm::asin(_front.y));

	this->updateCameraVector();
	glCheckError();
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
	const glm::vec3 vAxis(0.0f, 1.0f, 0.0f);

	glm::vec3 view(1.0f, 0.0f, 0.0f);
	view = rotate(vAxis, _yaw, view);
	view = glm::normalize(view);

	glm::vec3 hAxis = glm::cross(vAxis, view);
	hAxis = glm::normalize(hAxis);
	view = rotate(hAxis, _pitch, view);
	view = glm::normalize(view);

	_front = view;

	_up = glm::cross(_front, hAxis);
	_up = glm::normalize(_up);

	_right = glm::cross(_front, _up);
	_right = glm::normalize(_right);

	glCheckError();
}

void Camera::updateUniformBuffer()
{
	glm::mat4 view = getViewMatrix();
	glm::mat4 projection = getProjectionMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, this->_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(projection), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glCheckError();
}

void Camera::mouseMoveView(GLfloat xOffset_, GLfloat yOffset_, GLboolean constraiPitch_)
{
	this->_yaw += xOffset_;
	this->_pitch -= yOffset_;

	if (!constraiPitch_)
	{
		if (this->_pitch > 89.0f)
		{
			this->_pitch = 89.0f;
		}
		else if (this->_pitch < -89.0f)
		{
			this->_pitch = -89.0f;
		}
	}
}

void Camera::mouseMoveLength(GLfloat xOffset_, GLfloat yOffset_)
{
	this->_eye -= glm::vec3(xOffset_, -yOffset_, 0);
}
