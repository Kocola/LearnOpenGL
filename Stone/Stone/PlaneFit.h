#ifndef					__PLANE_FIT_H__
#define					__PLANE_FIT_H__

#include <vector>

#include "Common.h"

class StelaRubb;

class PlaneFit
{
	friend class StelaRubb;

	struct Plane
	{
		/* z = ax + by + c */
		Plane(float a_, float b_, float c_);

		float calcZ(float x_, float y_) const; 

		float calcZDiff(const glm::vec3& point_) const;

		float a, b, c;
	};
public:
	PlaneFit(int numX_, int numY_, const std::vector<glm::vec3>& points_,
		const std::vector<MeshFace>& faces_);

	std::vector<glm::vec3> calcPointColor();

private:
	void initBoundary();

	void divideArea(const std::vector<MeshFace>& faces_);

	//计算拟合平面
	Plane calcFitPlane(const std::vector<MeshFace>& faces_);

	void calcPointColor(const std::vector<MeshFace>& faces_, const Plane& plane_);

private:
	int														_numX;
	int														_numY;
	float														_minX;
	float														_maxX;
	float														_minY;
	float														_maxY;
	std::vector<glm::vec3>							_points;
	//std::vector<MeshFace>							_faces;
	std::vector<glm::vec3>							_colors;
	std::vector<std::vector<MeshFace>>		_facesVec;
};

#endif