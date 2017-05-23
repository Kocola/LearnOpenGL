#ifndef					__MODEL_FILE_H__
#define					__MODEL_FILE_H__

#include <fstream>
#include <vector>

#include "Common.h"

class ModelFile
{
public:
	ModelFile();

	ModelFile(const char* path_);
	
public:
	void open(const char* path_);

	void close();

	std::vector<glm::vec3> readVertexs();

	std::vector<MeshFace> readFaces();

private:
	void readHead();

private:
	std::ifstream			_ifstream;
	unsigned int			_vertexNum;
	unsigned int			_faceNum;
};

#endif