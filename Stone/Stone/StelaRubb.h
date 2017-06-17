#ifndef				__STELA_RUBB_H__
#define				__STELA_RUBB_H__

#include <vector>

#include "Application.h"
#include "Common.h"
#include "PlaneFit.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Plane;

class StelaRubb : public Application
{
public:
	StelaRubb();

	~StelaRubb();

private:
	bool init();

	bool initVertex();

	bool bindVertex();

	void setZ(float z_);

	glm::mat4 calcModelMatrix(const PlaneFit& planeFit_);

private:
	void initTexture();

	void initFBO();

	void initShaderProgram();

	void initShape();

protected:
	void draw() override;

	void draw1();

private:
	void firstDraw();

	void secondDraw();

private:
	GLuint				_VAO;
	glm::mat4			_modelMatrix;

	GLuint				_FBO;
	Texture2D			_depthTexture;
	Texture2D			_colorTexture;
	Plane*				_plane;
	ShaderProgram	_postProcessShaderProgram;	//后期处理shader

	std::vector<glm::vec3>			_points;
	std::vector<glm::vec3>			_colors;
	std::vector<MeshFace>			_faces;
};

#endif