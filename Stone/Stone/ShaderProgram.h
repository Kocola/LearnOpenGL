#ifndef					__SHADER_PROGRAM_H__
#define					__SHADER_PROGRAM_H__

#include "Common.h"

class ShaderProgram
{
public:
	enum ShaderType{
		Vertex,
		Fragment,
		Geometry
	};

public:
	ShaderProgram();

	~ShaderProgram();

public:
	bool addShaderFromSourceCode(const ShaderType& type_, 
		const GLchar* sourceCode_);

	bool link();

	bool use() const;

	void release() const;

public:
	void setUniformValue(const GLchar* name_, GLfloat value_);

	void setUniformValue(const GLchar* name_, GLsizei vectorLen_, GLsizei count_, const GLfloat* values_);

	void setUniformValue(const GLchar* name_, GLsizei vectorLen_, GLsizei count_, const GLint* values_);

	void setUniformValue(const GLchar* name_, GLint value_);

	void setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_);

	void setUniformValue(const GLchar* name_, const glm::vec2& value_);

	void setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_, GLfloat z_);

	void setUniformValue(const GLchar* name_, const glm::vec3& value_);

	void setUniformValue(const GLchar* name_, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_);

	void setUniformValue(const GLchar* name_, glm::vec4& value_);

	void setUniformValue(const GLchar* name_, const glm::mat3& value_);

	void setUniformValue(const GLchar* name_, const glm::mat4& value_);

public:
	GLint uniformLocation(const GLchar* name_) const;

	void setUniformBlockBind(const GLchar* uboName_, GLuint bindPoint_);

private:
	bool checkCompileErrors(GLuint shader_);

	bool checkLinkErrors(GLuint program_);

private:
	GLuint			_programId;
	GLuint			_vShader;
	GLuint			_fShader;
	GLuint			_gShader;
};

#endif