#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ResourceManager.h"

std::map<std::string, ShaderProgram> ResourceManager::s_shaderProgramMap;

std::map<std::string, Texture2D> ResourceManager::s_texture2DMap;

std::map<std::string, CubeMap> ResourceManager::s_cubeMap;

void ResourceManager::initResource()
{
	loadShaderProgram("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/stelaRubb.vs", "shaders/stelaRubb.frag", nullptr, "stelaRubb")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/light/basic.vs", "shaders/light/lightSource.frag", nullptr, "lightSource")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/light/basic.vs", "shaders/light/basicLight.frag", nullptr, "basicLight")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/light/basic.vs", "shaders/light/pointLight.frag", nullptr, "pointLight")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/light/basic.vs", "shaders/light/directionalLight.frag", nullptr, "directionalLight")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/light/basic.vs", "shaders/light/spotLight.frag", nullptr, "spotLight")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/depth.vs", "shaders/advancedOpenGL/depth.frag", nullptr, "depth")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/depth.vs", "shaders/advancedOpenGL/linearDepth.frag", nullptr, "linearDepth")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/stencil.vs", "shaders/advancedOpenGL/stencil.frag", nullptr, "stencil")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/framebuffer.vs", "shaders/advancedOpenGL/framebuffer.frag", nullptr, "framebuffer");
		//->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/cubeMap.vs", "shaders/advancedOpenGL/cubeMap.frag", nullptr, "skybox")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedOpenGL/simpleDepth.vs", "shaders/advancedOpenGL/simpleDepth.frag", nullptr, "simpleDepth"); 
	loadShaderProgram("shaders/advancedOpenGL/simpleShadow.vs", "shaders/advancedOpenGL/simpleShadow.frag", nullptr, "simpleShadow")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/pointDepth.vs", "shaders/advancedLight/pointDepth.frag", "shaders/advancedLight/pointDepth.gs", "pointDepth");
		//->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/pointShadow.vs", "shaders/advancedLight/pointShadow.frag", nullptr, "pointShadow")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/pointShadow.vs", "shaders/advancedLight/pointShadow2.frag", nullptr, "pointShadow2")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/normalMap/simpleNormalMap.vs", "shaders/advancedLight/normalMap/simpleNormalMap.frag", nullptr, "simpleNormalMap")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/normalMap/normalMap.vs", "shaders/advancedLight/normalMap/normalMap.frag", nullptr, "normalMap")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/normalMap/normalMap2.vs", "shaders/advancedLight/normalMap/normalMap2.frag", nullptr, "normalMap2")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/parallaxMap/parallaxMap.vs", "shaders/advancedLight/parallaxMap/parallaxMap.frag", nullptr, "parallaxMap")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/parallaxMap/parallaxMap.vs", "shaders/advancedLight/parallaxMap/steepParallaxMap.frag", nullptr, "steepParallaxMap")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/parallaxMap/parallaxMap.vs", "shaders/advancedLight/parallaxMap/steepParallaxMap2.frag", nullptr, "steepParallaxMap2")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/parallaxMap/parallaxMap.vs", "shaders/advancedLight/parallaxMap/parallaxOcclusionMap.frag", nullptr, "parallaxOcclusionMap")
		->setUniformBlockBind("Camera", CameraUniformBindPoint);
	loadShaderProgram("shaders/advancedLight/hdr/hdr.vs", "shaders/advancedLight/hdr/hdr.frag", nullptr, "hdr");
	loadShaderProgram("shaders/advancedLight/hdr/hdr.vs", "shaders/advancedLight/hdr/exposureHdr.frag", nullptr, "exposureHdr");

	loadTexture2D("resources/textures/moss/mossgrown_normal.png", GL_FALSE, "normalMoss");
	loadTexture2D("resources/textures/moss/mossgrown.png", GL_FALSE, "moss");
	loadTexture2D("resources/textures/planet_textures/texture_earth_clouds.jpg", GL_FALSE, "earth");
	loadTexture2D("resources/textures/planet_textures/texture_sun.jpg", GL_FALSE, "sun");
	loadTexture2D("resources/textures/planet_textures/texture_moon.jpg", GL_FALSE, "moon");
	loadCubeMap("resources/textures/skybox/", GL_FALSE, "cubemap");
	loadTexture2D("resources/textures/normalMap/brickwall.jpg", GL_FALSE, "brickWall");
	loadTexture2D("resources/textures/normalMap/brickwall_normal.jpg", GL_FALSE, "normalBrickWall");
	loadTexture2D("resources/textures/parallaxMap/bricks2.jpg", GL_FALSE, "parallaxBricks");
	loadTexture2D("resources/textures/parallaxMap/bricks2_depth.jpg", GL_FALSE, "parallaxBricksDepth");
	loadTexture2D("resources/textures/parallaxMap/bricks2_normal.jpg", GL_FALSE, "parallaxBricksNormal");
}

ShaderProgram* ResourceManager::loadShaderProgram(const GLchar* vShaderFile_,
	const GLchar* fShaderFile_, const GLchar* gShaderFile_, const std::string& shaderName_)
{
	ShaderProgram shaderProgram;
	std::string vShaderCode = loadShaderFile(vShaderFile_);
	if (fShaderFile_ == nullptr || shaderProgram.addShaderFromSourceCode(ShaderProgram::Vertex, vShaderCode.c_str()) == false)
	{
		std::cout << vShaderFile_ << std::endl;
		std::cout << __FILE__ << "," << __LINE__ << std::endl; 
		std::cout << "顶点着色器加载失败!" << std::endl;
	}
	std::string fShaderCode = loadShaderFile(fShaderFile_);
	if (shaderProgram.addShaderFromSourceCode(ShaderProgram::Fragment, fShaderCode.c_str()) == false)
	{
		std::cout << fShaderFile_ << std::endl;
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << "片段着色器加载失败!" << std::endl;
	}
	glCheckError();
	if (gShaderFile_ != nullptr)
	{
		std::string gShaderCode = loadShaderFile(gShaderFile_);
		if (shaderProgram.addShaderFromSourceCode(ShaderProgram::Geometry, gShaderCode.c_str()) == false)
		{
			std::cout << gShaderFile_ << std::endl;
			std::cout << __FILE__ << "," << __LINE__ << std::endl;
			std::cout << "几何着色器加载失败!" << std::endl;
		}
	}
	glCheckError();
	if (!shaderProgram.link())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << "着色器程序链接失败!" << std::endl;
	}
	s_shaderProgramMap[shaderName_] = shaderProgram;
	glCheckError();
	return getShaderProgramPointer(shaderName_);
}

ShaderProgram* ResourceManager::getShaderProgramPointer(const std::string& shaderName_)
{
	return &s_shaderProgramMap[shaderName_];
}

ShaderProgram ResourceManager::getShaderProgram(const std::string& shaderName_)
{
	if (s_shaderProgramMap.find(shaderName_) == s_shaderProgramMap.end())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << shaderName_ << ", 该着色器程序不存在!" << std::endl;
		throw 1;
	}
	return s_shaderProgramMap[shaderName_];
}

Texture2D* ResourceManager::loadTexture2D(const GLchar* path_, GLboolean alpha_,
	const std::string& name_)
{
	s_texture2DMap[name_] = loadTextureFromFile(path_, alpha_);
	return getTexture2DPointer(name_);
}

Texture2D* ResourceManager::getTexture2DPointer(const std::string& name_)
{
	if (s_texture2DMap.find(name_) == s_texture2DMap.end())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << name_ << ", 该纹理名不存在!" << std::endl;
		throw 1;
	}
	return &s_texture2DMap[name_];
}

Texture2D ResourceManager::getTexture2D(const std::string& name_)
{
	if (s_texture2DMap.find(name_) == s_texture2DMap.end())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << name_ << ", 该纹理名不存在!" << std::endl;
		throw 1;
	}
	return s_texture2DMap[name_];
}

CubeMap ResourceManager::loadCubeMap(const GLchar* parentPath_, 
	GLboolean alpha_, const std::string& name_)
{
	if (s_cubeMap.find(name_) != s_cubeMap.end())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << name_ << ", 该立方体纹理名已存在!" << std::endl;
		throw 1;
	}
	s_cubeMap[name_] = loadCubeMapFromFile(parentPath_, alpha_);
	return s_cubeMap[name_];
}

CubeMap ResourceManager::getCubeMap(const std::string& name_)
{
	if (s_cubeMap.find(name_) == s_cubeMap.end())
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << name_ << ", 该立方体纹理名不存在!" << std::endl;
		throw 1;
	}
	return s_cubeMap[name_];
}

std::string ResourceManager::loadShaderFile(const char* path_)
{
	if (path_ == nullptr)
	{
		return std::string();
	}

	std::ifstream fstream(path_);
	if (!fstream.is_open())
	{
		std::cout << __FILE__ << ":" << __LINE__ << std::endl;
		std::cout << path_ << "：文件打开失败!" << std::endl;
		throw 1;
	}

	std::stringstream sstream;
	sstream << fstream.rdbuf();

	fstream.clear();

	fstream.close();

	return sstream.str();
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* path_,
	GLboolean alpha_)
{
	Texture2D texture2d;

	if (alpha_)
	{
		texture2d.setInternalFormat(GL_RGBA);
		texture2d.setImageFormat(GL_RGBA);
	}

	int width = 0, height = 0;
	unsigned char* image = SOIL_load_image(path_,
		&width, &height, 0, texture2d.getInternalFormat() != GL_RGBA ?
	SOIL_LOAD_RGB : SOIL_LOAD_RGBA);

	if (image == nullptr)
	{
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
		std::cout << "打开图像文件:"<<path_<<" 失败!" << std::endl;
		throw 1;
	}

	texture2d.generate(width, height, image);
	SOIL_free_image_data(image);

	return texture2d;
}

CubeMap ResourceManager::loadCubeMapFromFile(const GLchar* path_, GLboolean alpha_)
{
	CubeMap cubeMap;
	if (alpha_)
	{
		cubeMap.setInternalFormat(GL_RGBA);
		cubeMap.setImageFormat(GL_RGBA);
	}

	std::vector<Image> images;
	std::vector<std::string> fileNames{
		"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"
	};
	for (int i = 0; i < 6; ++i)
	{
		std::string fileName = path_ + fileNames[i];
		Image image;
		image.data = SOIL_load_image(fileName.c_str(), &image.width, &image.height,
			0, cubeMap.getInternalFormat() == GL_RGB ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);
		if (image.data == nullptr)
		{
			std::cout << __FILE__ << ", " << __LINE__ << std::endl;
			std::cout << "打开图像文件:"<<fileName<<" 失败!" << std::endl;
			throw 1;
		}
		images.push_back(image);
	}
	cubeMap.generate(images);

	for (int i = 0; i < 6; ++i)
	{
		SOIL_free_image_data(images[i].data);
	}

	return cubeMap;
}