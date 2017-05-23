#include <iostream>

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
	: _vShader(0), _fShader(0), _gShader(0), _programId(0)
{
}

ShaderProgram::~ShaderProgram()
{
// 	if (glIsProgram(_programId))
// 	{
// 		glDeleteProgram(_programId);
// 	}
}

bool ShaderProgram::addShaderFromSourceCode(const ShaderType& type_, 
	const GLchar* sourceCode_)
{
	GLuint* shader = nullptr;

	GLenum shaderType;
	if (type_ == ShaderType::Vertex)
	{
		shaderType = GL_VERTEX_SHADER;
		shader = &_vShader;
	}
	else if (type_ == ShaderType::Fragment)
	{
		shaderType = GL_FRAGMENT_SHADER;
		shader = &_fShader;
	}
	else if (type_ == ShaderType::Geometry)
	{
		shaderType = GL_GEOMETRY_SHADER;
		shader = &_gShader;
	}

	*shader = glCreateShader(shaderType);
	glShaderSource(*shader, 1, &sourceCode_, nullptr);
	glCompileShader(*shader);

	return checkCompileErrors(*shader);
}

bool ShaderProgram::link()
{
	//如果没有创建顶点着色器和片段着色器
	//那么直接返回链接失败
	if (!(_vShader && _fShader))
	{
		return false;
	}

	this->_programId = glCreateProgram();
	glAttachShader(this->_programId, _vShader);
	glAttachShader(this->_programId, _fShader);
	if (_gShader != 0)
	{
		glAttachShader(this->_programId, _gShader);
	}
	glLinkProgram(this->_programId);

	glDeleteShader(_vShader);
	glDeleteShader(_fShader);
	if (_gShader != 0)
	{
		glDeleteShader(_gShader);
	}

	return checkLinkErrors(_programId);
}

bool ShaderProgram::checkCompileErrors(GLuint shader_)
{
	if (glIsShader(shader_) == GL_FALSE)
	{
		return false;
	}

	GLint isSuccess;
	GLchar logInfo[1024];

	glGetShaderiv(shader_, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(shader_, sizeof(logInfo), nullptr, logInfo);
		std::cout << "| ERROR:SHADER::Compile-Time Error : \n"
			<< logInfo << "\n -------------------- \n"
			<< std::endl;
	}

	return isSuccess;
}

bool ShaderProgram::checkLinkErrors(GLuint program_)
{
	if (glIsProgram(program_) == GL_FALSE)
	{
		return false;
	}

	GLint isSuccess;
	GLchar logInfo[1024];

	glGetProgramiv(program_, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(program_, sizeof(logInfo), nullptr, logInfo);
		std::cout << "| ERROR:PROGRAM::Link-Time Error : \n"
			<< logInfo << "\n -------------------- \n"
			<< std::endl;
	}

	return isSuccess;
}

bool ShaderProgram::use() const
{
	if (glIsProgram(_programId) == GL_FALSE)
	{
		return false;
	}
	glUseProgram(_programId);
	return true;
}

void ShaderProgram::release() const
{
	glUseProgram(0);
}

GLint ShaderProgram::uniformLocation(const GLchar* name_) const
{
	//如果没有绑定当前的program，那么glUniformMatrix4fv
	//操作会报出 GL_INVALID_VALUE 错误
	this->use();

	auto location = glGetUniformLocation(this->_programId, name_);

	return location;
}

void ShaderProgram::setUniformValue(const GLchar* name_, GLint value_)
{
	auto location = uniformLocation(name_);
	glUniform1i(location, value_);
}

void ShaderProgram::setUniformValue(const GLchar* name_, GLfloat value_)
{
	auto location = uniformLocation(name_);
	glUniform1f(location, value_);
}

void ShaderProgram::setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_)
{
	auto location = uniformLocation(name_);
	glUniform2f(location, x_, y_);
}

void ShaderProgram::setUniformValue(const GLchar* name_, const glm::vec2& value_)
{
	setUniformValue(name_, value_.x, value_.y);
}

void ShaderProgram::setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_, GLfloat z_)
{
	auto location = uniformLocation(name_);
	glUniform3f(location, x_, y_, z_);
}

void ShaderProgram::setUniformValue(const GLchar* name_, const glm::vec3& value_)
{
	setUniformValue(name_, value_.x, value_.y, value_.z);
}

void ShaderProgram::setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_)
{
	auto location = uniformLocation(name_);
	glUniform4f(location, x_, y_, z_, w_);
}

void ShaderProgram::setUniformValue(const GLchar* name_, glm::vec4& value_)
{
	setUniformValue(name_, value_.x, value_.y, value_.z, value_.w);
}

void ShaderProgram::setUniformValue(const GLchar* name_, const glm::mat3& value_)
{
	auto location = uniformLocation(name_);
	//GLM的默认布局是列主序，因此不需要置换矩阵，故是GL_FALSE
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value_));
}

void ShaderProgram::setUniformValue(const GLchar* name_, const glm::mat4& value_)
{
	auto location = uniformLocation(name_);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value_));
}

void ShaderProgram::setUniformBlockBind(const GLchar* uboName_,
	GLuint bindPoint_)
{
	GLuint uniformBlockIndex = glGetUniformBlockIndex(_programId, uboName_);
	glUniformBlockBinding(_programId, uniformBlockIndex, bindPoint_);
}