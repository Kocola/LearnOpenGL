#ifndef					__RESOURCE_MANAGER_H__
#define					__RESOURCE_MANAGER_H__

#include <map>
#include <string>

#include "CubeMap.h"
#include "Singleton.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class ResourceManager : public Singleton < ResourceManager >
{
public:
	//初始化所有必须的资源
	void initResource();

public:
	static ShaderProgram* loadShaderProgram(const GLchar* vShaderFile_,
		const GLchar* fShaderFile_, const GLchar* gShaderFile_, const std::string& shaderName_);

	static ShaderProgram* getShaderProgramPointer(const std::string& shaderName_);

	static ShaderProgram getShaderProgram(const std::string& shaderName_);

	static Texture2D* loadTexture2D(const GLchar* path_, GLboolean alpha_,
		const std::string& name_);

	static Texture2D* getTexture2DPointer(const std::string& name_);

	static Texture2D getTexture2D(const std::string& name_);

	static CubeMap loadCubeMap(const GLchar* parentPath_,
		GLboolean alpha_, const std::string& name_);

	static CubeMap getCubeMap(const std::string& name_);

private:
	static std::string loadShaderFile(const char* path_);

	static Texture2D loadTextureFromFile(const GLchar* path_, GLboolean alpha_);

	static CubeMap	 loadCubeMapFromFile(const GLchar* path_, GLboolean alpha_);

private:
	static std::map<std::string, ShaderProgram> s_shaderProgramMap;

	static std::map<std::string, Texture2D> s_texture2DMap;

	static std::map<std::string, CubeMap> s_cubeMap;
};

#endif