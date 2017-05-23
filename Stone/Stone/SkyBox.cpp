#include "CubeMesh.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "SkyBox.h"

SkyBox::SkyBox(const float sideLen_ /* = 1.0f */)
	:_sideLen(sideLen_), _renderInfo(Mesh<CubeMesh>::getInstance().getRenderInfo())
{
	if (glIsVertexArray(_renderInfo.VAO) == GL_FALSE ||
		glIsBuffer(_renderInfo.VBO) == GL_FALSE)
	{
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
		std::cout << "VAO��VEO������!" << std::endl;
		throw 1;
	}

	_cubeMap = ResourceManager::getInstance().getCubeMap("cubemap");
	_shaderProgram = ResourceManager::getInstance().getShaderProgram("skybox");
}

SkyBox::~SkyBox()
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

void SkyBox::draw()
{	
	//glDepthFunc(GL_LEQUAL);
	//glDepthMask(GL_FALSE);
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(_sideLen));
	_shaderProgram.setUniformValue("model", model);
	_shaderProgram.use();
	_cubeMap.bind();
	render();
	_cubeMap.release();
	_shaderProgram.release();
	//glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);
}

void SkyBox::render()
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

void SkyBox::setSideLen(const float sideLen_)
{
	this->_sideLen = sideLen_;
}

void SkyBox::setCubeMap(const CubeMap& cubeMap_)
{
	this->_cubeMap = cubeMap_;
}