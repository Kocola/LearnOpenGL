#ifndef					__PBR_H__
#define					__PBR_H__

/*	基于物理的渲染 ---2017.6.17
*/

#include <vector>

#include "Application.h"
#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Shape;

class PBR	: public Application
{
public:
	PBR();

	~PBR();

public:
	virtual void draw() override;

private:
	void init();

	void initParameter();

	void initTexture();

	void initShaderProgram();

	void initShape();

private:
	void drawWithoutTexture();	//绘制无纹理

	void drawWithTexture(); //绘制有纹理

private:
	std::vector<glm::vec3>		_lightPositions;
	std::vector<glm::vec3>		_lightColors;

	int									_rows;	//球体矩阵行数
	int									_columns;		//球体矩阵列数
	int									_intervals;		//球体矩阵球体之间的间隙
	std::vector<Shape*>			_spheres;	//球体指针容器，避免每次绘制重复构造球体

	std::vector<Shape*>			_lights;

	ShaderProgram					_pbrShaderProgramWithoutTexture;
	ShaderProgram					_pbrShaderProgramWithTexture;

	Texture2D							_normalTexture;
	Texture2D							_albedoTexture;
	Texture2D							_metallicTexture;
};

#endif