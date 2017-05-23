#include <iostream>
#include <sstream>
#include <string>

#include "ModelFile.h"

ModelFile::ModelFile()
	: _vertexNum(0), _faceNum(0)
{
}

ModelFile::ModelFile(const char* path_)
	: ModelFile()
{
	open(path_);
}

void ModelFile::open(const char* path_)
{
	//关闭已经打开的文件
	if (_ifstream.is_open())
	{
		_ifstream.close();
	}
	_ifstream.open(path_, std::ios::in | std::ios::binary);
	if (!_ifstream.is_open())
	{
		std::cout << "Open File Failure!" << std::endl;
		exit(0);
	}
	readHead();
}

void ModelFile::close()
{
	if (_ifstream.is_open())
	{
		_ifstream.close();
	}
}

void ModelFile::readHead()
{
	char chs[50];

	do{
		_ifstream.getline(chs, sizeof(chs));
		std::string line(chs);
		//std::istringstream isstream(std::string(chs));
		std::istringstream isstream(line);
		std::string str;
		if (isstream >> str)
		{
			if (str != "element") continue;
			std::string number;
			isstream >> str >> number;
			if (str == "vertex")
			{
				this->_vertexNum = std::atoi(number.c_str());
			} 
			else if (str == "face")
			{
				this->_faceNum = std::atoi(number.c_str());
			}
		}
	} while (std::string(chs) != "end_header" || std::string(chs).empty());
}

std::vector<glm::vec3> ModelFile::readVertexs()
{
	assert(_vertexNum != 0);

	char vertex[4];
	std::vector<glm::vec3> vertexs;
	vertexs.reserve(_vertexNum);
	for (int i = 0; i < _vertexNum; ++i)
	{
		float points[3];
		for (int k = 0; k < 3; ++k)
		{
			_ifstream.read(vertex, 4);
			float* tmp = reinterpret_cast<float*>(vertex);
			points[k] = *tmp;
		}
		vertexs.push_back(glm::vec3(points[0], points[1], points[2]));
//		vertexs.push_back(glm::vec3(points[0], points[1], 0));
	}
	return std::move(vertexs);
}

std::vector<MeshFace> ModelFile::readFaces()
{
	assert(_faceNum != 0);

	char face[4];
	std::vector<MeshFace> faces;
	faces.reserve(_faceNum);
	for (int i = 0; i < _faceNum; ++i)
	{
		char faceNum;
		_ifstream.read(&faceNum, 1);
		unsigned int indexs[3];
		for (int k = 0; k < 3; ++k)
		{
			_ifstream.read(face, 4);
			unsigned int* index = reinterpret_cast<unsigned int*>(face);
			indexs[k] = *index;
		}
		faces.push_back(MeshFace(indexs[0], indexs[1], indexs[2]));
	}
	return faces;
}