#ifndef					__TEXT_H__
#define					__TEXT_H__

/*	�ı���Ⱦ	---	2017.6.17
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
		Texture2D		texture;		//��������
		glm::ivec2		size;			//���δ�С
		glm::ivec2		bearing;	//�ӻ�׼�ߵ�������/������ƫ��ֵ
		GLuint			advance;	//ԭ�������һ������ԭ��ľ���
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

	void initRender();	//��ʼ����Ⱦ����

	void initShaderProgram();	//��ʼ����ɫ������

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