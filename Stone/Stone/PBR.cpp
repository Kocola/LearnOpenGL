#include "PBR.h"
#include "ResourceManager.h"
#include "Sphere.h"

PBR::PBR()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "PBR")
{
	init();
}

PBR::~PBR()
{
	for (int i = 0; i < _spheres.size(); ++i)
	{
		if (_spheres[i] != nullptr)
		{
			delete _spheres[i];
			_spheres[i] = nullptr;
		}
	}

	for (int i = 0; i < _lights.size(); ++i)
	{
		if (_lights[i] != nullptr)
		{
			delete _lights[i];
			_lights[i] = nullptr;
		}
	}
}

void PBR::init()
{
	initParameter();
	initTexture();
	initShaderProgram();
	initShape();
}

void PBR::initParameter()
{
	//初始化光源位置，共4个
	_lightPositions.push_back(glm::vec3(-10.0f, 10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(10.0f, 10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(-10.0f, -10.0f, 10.0f));
	_lightPositions.push_back(glm::vec3(10.0f, -10.0f, 10.0f));

	//指定光源颜色，共4个
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	_lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));

	//初始化球体矩阵参数
	_rows = 7;
	_columns = 7;
	_intervals = 3.5;
}

void PBR::initTexture()
{
	_normalTexture = ResourceManager::getInstance().getTexture2D("pbrNormal");
	_metallicTexture = ResourceManager::getInstance().getTexture2D("pbrMetallic");
	_albedoTexture = ResourceManager::getInstance().getTexture2D("pbrAlbedo");
}

void PBR::initShaderProgram()
{
	glCheckError();

	_pbrShaderProgramWithoutTexture = ResourceManager::getInstance().getShaderProgram("pbr1");
	_pbrShaderProgramWithoutTexture.setUniformValue("albedo", 0.5f, 0.0f, 0.f);		//指定材质表面颜色或基础反射率
	_pbrShaderProgramWithoutTexture.setUniformValue("ao", 1.0f);	//指定环境光遮蔽

	_pbrShaderProgramWithTexture = ResourceManager::getInstance().getShaderProgram("pbr2");

	glCheckError();
}

void PBR::initShape()
{
	glCheckError();

	//初始化球体
	for (int i = 0; i < _rows; ++i)
	{
		for (int k = 0; k < _columns; ++k)
		{
			auto sphere = new Sphere();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(float)((k - _columns / 2) * _intervals),
				(float)((i - _rows / 2) * _intervals),
				0.0f
			));
			sphere->setModelMatrix(model);
			_spheres.push_back(sphere);
		}
	}

	//初始化光源物体
	for (int i = 0; i < _lightPositions.size(); ++i)
	{
		Shape* sphere = new Sphere();
		sphere->setTexture2D(ResourceManager::getInstance().getTexture2D("white"));
		_lights.push_back(sphere);
	}

	glCheckError();
}

void PBR::draw()
{
	//drawWithoutTexture();

	drawWithTexture();
}

void PBR::drawWithoutTexture()
{
	glCheckError();

	_pbrShaderProgramWithoutTexture.setUniformValue("viewPos", getCameraPosition());

	//设置光源位置
	for (int i = 0; i < _lightPositions.size(); ++i)
	{
		glm::vec3 newPos = _lightPositions[i] + glm::vec3(glm::sin(glfwGetTime() * 5.0f) * 5.0f, 0.0, 0.0);
		_pbrShaderProgramWithoutTexture.setUniformValue((std::string("lightPositions[") + std::to_string(i) + std::string("]")).c_str(),
			newPos);
		_pbrShaderProgramWithoutTexture.setUniformValue((std::string("lightColors[") + std::to_string(i) + std::string("]")).c_str(),
			_lightColors[i]);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		_lights[i]->setModelMatrix(model);
		_lights[i]->draw();
	}

	for (int i = 0; i < _rows; ++i)
	{
		_pbrShaderProgramWithoutTexture.setUniformValue("metallic", (float)i / (float)_rows);
		for (int k = 0; k < _columns; ++k)
		{
			_pbrShaderProgramWithoutTexture.setUniformValue("roughness", glm::clamp((float)k / (float)_columns, 0.05f, 1.0f));
			auto sphere = _spheres[i*_columns + k];
			sphere->setShaderProgram(_pbrShaderProgramWithoutTexture);
			sphere->draw();
		}
	}

	glCheckError();
}

void PBR::drawWithTexture()
{
	glCheckError();

	_pbrShaderProgramWithTexture.setUniformValue("viewPos", getCameraPosition());

	//设置光源位置
	for (int i = 0; i < _lightPositions.size(); ++i)
	{
		glm::vec3 newPos = _lightPositions[i] + glm::vec3(glm::sin(glfwGetTime() * 5.0f) * 5.0f, 0.0, 0.0);
		_pbrShaderProgramWithTexture.setUniformValue((std::string("lightPositions[") + std::to_string(i) + std::string("]")).c_str(),
			newPos);
		_pbrShaderProgramWithTexture.setUniformValue((std::string("lightColors[") + std::to_string(i) + std::string("]")).c_str(),
			_lightColors[i]);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		_lights[i]->setModelMatrix(model);
		_lights[i]->draw();
	}
	
	for (int i = 0; i < _rows; ++i)
	{
		for (int k = 0; k < _columns; ++k)
		{
			auto sphere = _spheres[i*_columns + k];
			sphere->setShaderProgram(_pbrShaderProgramWithTexture);
			sphere->setTexture2D(std::vector < Texture2D > {_normalTexture, _metallicTexture, _albedoTexture},
				std::vector<std::string> { "normalMap", "metallicMap", "albedoMap" });
			sphere->draw();
		}
	}

	glCheckError();
}
