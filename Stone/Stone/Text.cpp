#include "Text.h"

#include "Common.h"
#include "ResourceManager.h"

Text::Text()
{
}

Text::~Text()
{
}

Text* Text::getInstance()
{
	static Text text;
	return &text;
}

void Text::init()
{
	initText();
	initCharactersMap();
	initRender();
	initShaderProgram();
}

void Text::initText()
{
	if (FT_Init_FreeType(&_ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		throw 1;
	}

	if (FT_New_Face(_ft, "resources/fonts/arial.ttf", 0, &_face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		throw 1;
	}

	//设置字体面的宽度和高度，将宽度值设为0，
	//表示我们要从字体面通过给定的高度中动态计算出字形的宽度
	FT_Set_Pixel_Sizes(_face, 0, 48);
}

void Text::initCharactersMap()
{
	glCheckError();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		//禁止字节对齐限制

	for (GLubyte c = 0; c < 128; ++c)
	{
		//加载字符的字形
		if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		Texture2D texture;
		texture.setWrapType(GL_CLAMP_TO_EDGE);
		texture.setInternalFormat(GL_RED);
		texture.setImageFormat(GL_RED);
		texture.setDataType(GL_UNSIGNED_BYTE);
		texture.generate(_face->glyph->bitmap.width, _face->glyph->bitmap.rows,
			_face->glyph->bitmap.buffer);
		//储存字符供以后使用
		Character character = {
			texture,
			glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
			glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
			_face->glyph->advance.x
		};
		_charactersMap.insert(std::pair<GLchar, Character>(c, character));
	}

	//清理FreeType的资源
	FT_Done_Face(_face);
	FT_Done_FreeType(_ft);

	glCheckError();
}

void Text::initRender()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::initShaderProgram()
{
	_shaderProgram = ResourceManager::getInstance().getShaderProgram("font");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH),
		0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
	_shaderProgram.setUniformValue("projection", projection);
}

void Text::renderText(const std::string& text_, const glm::vec2 pos_, GLfloat scale_, const glm::vec3& color_)
{
	_shaderProgram.setUniformValue("textColor", color_);
	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_VAO);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat x = pos_.x;
	GLfloat y = pos_.y;

	for (int i = 0; i < text_.size(); ++i)
	{
		Character ch = _charactersMap[text_[i]];

		//计算字体左下角位置
		GLfloat xpos = x + ch.bearing.x * scale_;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale_;

		GLfloat w = ch.size.x;
		GLfloat h = ch.size.y;

		//对每个字符更新VAO（颠倒？）
		GLfloat vertices[6][4] = {
				{xpos,		ypos + h,	0.0,	0.0},
				{xpos,		ypos,			0.0,	1.0},
				{xpos + w,	ypos,			1.0,	1.0},

				{xpos,		ypos + h,	0.0,	0.0},
				{xpos + w,	ypos,			1.0,	1.0},
				{xpos + w,	ypos + h,	1.0,	0.0}
		};

		//Render glyph texture each character
		glBindTexture(GL_TEXTURE_2D, ch.texture.getTextureID());
		//更新VBO内存
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//绘制四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		x += (ch.advance >> 6) * scale_;
	}

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}