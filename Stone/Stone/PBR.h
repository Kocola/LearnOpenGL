#ifndef					__PBR_H__
#define					__PBR_H__

/*	�����������Ⱦ ---2017.6.17
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
	void drawWithoutTexture();	//����������

	void drawWithTexture(); //����������

private:
	std::vector<glm::vec3>		_lightPositions;
	std::vector<glm::vec3>		_lightColors;

	int									_rows;	//�����������
	int									_columns;		//�����������
	int									_intervals;		//�����������֮��ļ�϶
	std::vector<Shape*>			_spheres;	//����ָ������������ÿ�λ����ظ���������

	std::vector<Shape*>			_lights;

	ShaderProgram					_pbrShaderProgramWithoutTexture;
	ShaderProgram					_pbrShaderProgramWithTexture;

	Texture2D							_normalTexture;
	Texture2D							_albedoTexture;
	Texture2D							_metallicTexture;
};

#endif