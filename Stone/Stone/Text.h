#ifndef					__TEXT_H__
#define					__TEXT_H__

/*	文本渲染	---	2017.6.17
*/

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

#include "ShaderProgram.h"
#include "Singleton.h"
#include "Texture2D.h"

class Text : public Singleton<Text>
{
	struct Character
	{
		Texture2D		texture;		//字体纹理
		glm::ivec2		size;			//字形大小
		glm::ivec2		bearing;	//从基准线到字形左部/顶部的偏移值
		GLuint			advance;	//原点距离下一个字形原点的距离
	};

protected:
	Text();

	~Text();

public:
	static Text* getInstance();

	void init();

private:
	void initText();

	void initCharactersMap();

	void initRender();	//初始化渲染数据

	void initShaderProgram();	//初始化着色器程序

public:
	void renderText(const std::string& text_, const glm::vec2 pos_, GLfloat scale_,
		const glm::vec3& color_);

private:
	FT_Library									_ft;
	FT_Face										_face;
	std::map<GLchar, Character>		_charactersMap;

	GLuint										_VAO;
	GLuint										_VBO;
	ShaderProgram							_shaderProgram;
};

#endif