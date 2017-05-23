#include "Light.h"
#include "ResourceManager.h"
#include "Shape.h"

Shape::Shape(const RenderInfo& renderInfo_)
	:_renderInfo(renderInfo_), _shininess(32.0f),
	_modelMatrix(glm::mat4(1.0f)), _hasMultipleTextures(GL_FALSE)
{
	if (glIsVertexArray(_renderInfo.VAO) == GL_FALSE || 
		glIsBuffer(_renderInfo.VBO) == GL_FALSE)
	{
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
		std::cout << "VAO��VEO������!" << std::endl;
		throw 1;
	}

	_shaderProgram = ResourceManager::getInstance().getShaderProgram("basic");
	_texture2D = ResourceManager::getInstance().getTexture2D("moss");
	_texture2Ds.push_back(_texture2D);
	_textureNames.push_back("diffuseTexture");

/*	_lightColor = glm::vec3(1.0f);		//Ĭ�Ϲ�Դ��ɫΪ��ɫ*/
}

Shape::~Shape()
{
	//ɾ�����Դ��еĶ����������ݣ������ڴ�й¶
	if (glIsVertexArray(_renderInfo.VAO))
	{
		glDeleteVertexArrays(1, &_renderInfo.VAO);
	}
	//VBO������Ҳ���뼰ʱɾ�������������ڴ�й¶
	if (glIsBuffer(_renderInfo.VBO))
	{
		glDeleteBuffers(1, &_renderInfo.VBO);
	}
	//VEO����ɾ��
	if (glIsBuffer(_renderInfo.VEO))
	{
		glDeleteBuffers(1, &_renderInfo.VEO);
	}
}

void Shape::draw()
{
	_shaderProgram.setUniformValue("model", _modelMatrix);
	//_modelMatrix = glm::mat4(1.0f);
	_shaderProgram.setUniformValue("shininess", _shininess);
	_shaderProgram.use();
	glCheckError();
	for (int i = 0; i < _texture2Ds.size(); ++i)
	{
		_shaderProgram.setUniformValue(_textureNames[i].c_str(), i);
		glActiveTexture(GL_TEXTURE0 + i);
		_texture2Ds[i].bind();
	}
	//_texture2D.bind();
	glCheckError();
	render();
	glCheckError();
	for (int i = 0; i < _texture2Ds.size(); ++i)
	{
		_texture2Ds[i].release();
	}
	//_texture2D.release();
	_shaderProgram.release();
	glCheckError();
	//_modelMatrix = glm::mat4(1.0f);
}

void Shape::render()
{
	glBindVertexArray(_renderInfo.VAO);
	//���������VEO������Ĭ����������Ⱦ
	if (glIsBuffer(_renderInfo.VEO))
	{
		glDrawElements(GL_TRIANGLES, _renderInfo.count, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, _renderInfo.count);
	}
	glBindVertexArray(0);
}

void Shape::setTranslate(const glm::vec3& translate_)
{
	_modelMatrix = glm::translate(_modelMatrix, translate_);
}

void Shape::setRotate(GLfloat angle_, glm::vec3& center_)
{
	_modelMatrix = glm::rotate(_modelMatrix, angle_, center_);
}

void Shape::setScale(const glm::vec3& scale_)
{
	_modelMatrix = glm::scale(_modelMatrix, scale_);
}

void Shape::setModelMatrix(const glm::mat4& modelMatrix_)
{
	this->_modelMatrix = modelMatrix_;
}

void Shape::setShaderProgram(const ShaderProgram& shaderProgram_)
{
	this->_shaderProgram = shaderProgram_;
}

void Shape::setTexture2D(const Texture2D& texture2D_, const std::string& name_ /* = "diffuseTexture" */)
{
	this->_texture2D = texture2D_;
	_texture2Ds.clear();
	_texture2Ds.push_back(_texture2D);
	_textureNames.clear();
	_textureNames.push_back(name_);
}

void Shape::setTexture2D(const std::vector<Texture2D>& texture2Ds_, 
	const std::vector<std::string>& names_)
{
	this->_texture2Ds = texture2Ds_;
	this->_textureNames = names_;
}

void Shape::setShininess(GLfloat shininess_)
{
	this->_shininess = shininess_;
}

void Shape::setLightSource(Light* light_)
{
	if (light_ == nullptr)
	{
		return;
	}
	setShaderProgram(light_->getLightShader());
}
