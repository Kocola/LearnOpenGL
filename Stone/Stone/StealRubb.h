#include <vector>

#include "Application.h"
#include "Common.h"
#include "PlaneFit.h"

class StelaRubb : public Application
{
public:
	StelaRubb();

private:
	bool init();

	bool initVertex();

	bool bindVertex();

	void setZ(float z_);

	glm::mat4 calcModelMatrix(const PlaneFit& planeFit_);

protected:
	void draw() override;

private:
	GLuint				_VAO;
	//GLuint				_VEO;
	glm::mat4			_modelMatrix;

	std::vector<glm::vec3>			_points;
	std::vector<glm::vec3>			_colors;
	std::vector<MeshFace>			_faces;
};