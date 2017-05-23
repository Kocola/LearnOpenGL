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
		std::cout << "VAO或VEO不存在!" << std::endl;
		throw 1;
	}

	_cubeMap = ResourceManager::getInstance().getCubeMap("cubemap");
	_shaderProgram = ResourceManager::getInstance().getShaderProgram("skybox");
}

SkyBox::~SkyBox()
{
	//删除在显存中的顶点数组数据，避免内存泄露
	if (glIsVertexArray(_renderInfo.VAO))
	{
		glDeleteVertexArrays(1, &_renderInfo.VAO);
	}
	//VBO的数据也必须及时删除，否则会造成内存泄露
	if (glIsBuffer(_renderInfo.VBO))
	{
		glDeleteBuffers(1, &_renderInfo.VBO);
	}
	//VEO数据删除
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
	//如果建立了VEO，这里默认是索引渲染
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