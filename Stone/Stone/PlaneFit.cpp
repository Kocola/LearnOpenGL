#include <limits>

#include "PlaneFit.h"

PlaneFit::Plane::Plane(float a_, float b_, float c_)
	: a(a_), b(b_), c(c_)
{
}

float PlaneFit::Plane::calcZ(float x_, float y_) const
{
	return a * x_ + b * y_ + c;
}

float PlaneFit::Plane::calcZDiff(const glm::vec3& point_) const
{
	return point_.z - calcZ(point_.x, point_.y);
}

PlaneFit::PlaneFit(int numX_, int numY_, const std::vector<glm::vec3>& points_,
	const std::vector<MeshFace>& faces_)
	: _numX(numX_), _numY(numY_), _points(points_)
{
	_minX = std::numeric_limits<float>::max();
	_maxX = std::numeric_limits<float>::lowest();
	_minY = std::numeric_limits<float>::max();
	_maxY = std::numeric_limits<float>::lowest();

	_colors.resize(_points.size());

	initBoundary();
	divideArea(faces_);
}

void PlaneFit::initBoundary()
{
	for (int i = 0; i < _points.size(); ++i)
	{
		auto point = _points[i];
		if (point.x > _maxX)
		{
			_maxX = point.x;
		}
		if (point.x < _minX)
		{
			_minX = point.x;
		}

		if (point.y > _maxY)
		{
			_maxY = point.y;
		}
		if (point.y < _minY)
		{
			_minY = point.y;
		}
	}
}

void PlaneFit::divideArea(const std::vector<MeshFace>& faces_)
{
	_facesVec.resize(_numX * _numY);

	if (_facesVec.size() == 1)
	{
		_facesVec.front() = faces_;
		return;
	}

	float unitXLen = (_maxX - _minX) / _numX;
	float unitYLen = (_maxY - _minY) / _numY;

	for (int i = 0; i < faces_.size(); ++i)
	{
		auto face = faces_[i];
		auto p1Index = face.first;
		auto p2Index = face.second;
		auto p3Index = face.third;

		auto p1 = _points[p1Index];
		auto p2 = _points[p2Index];
		auto p3 = _points[p3Index];

		//第一个点输入哪个区域，那么整个面片都属于那个区域
		int xIndex = (p1.x - _minX) / unitXLen;
		int yIndex = (p1.y - _minY) / unitYLen;
		if (xIndex == _numX)
		{
			xIndex -= 1;
		}
		if (yIndex == _numY)
		{
			yIndex -= 1;
		}
		_facesVec[xIndex * _numY + yIndex].push_back(MeshFace(p1Index, p2Index, p3Index));
	}
}

std::vector<glm::vec3> PlaneFit::calcPointColor()
{
	for (int i = 0; i < _facesVec.size(); ++i)
	{
		auto plane = calcFitPlane(_facesVec[i]);
		calcPointColor(_facesVec[i], plane);
	}
	return _colors;
}

void PlaneFit::calcPointColor(const std::vector<MeshFace>& faces_,  
	const Plane& plane_)
{
	/* 计算最大最小diffZ */
	auto minDiffZ = std::numeric_limits<float>::max();
	auto maxDiffZ = std::numeric_limits<float>::lowest();
	for (int i = 0; i < faces_.size(); ++i)
	{
		std::vector<unsigned int> indexs{ faces_[i].first, faces_[i].second, faces_[i].third };
		for (int k = 0; k < indexs.size(); ++k)
		{
			auto tmp = plane_.calcZDiff(this->_points[indexs[k]]);
			minDiffZ = tmp < minDiffZ ? tmp : minDiffZ;
			maxDiffZ = tmp > maxDiffZ ? tmp : maxDiffZ;
		}
	}

	const auto DIFFZ = maxDiffZ - minDiffZ;
	for (int i = 0; i < faces_.size(); ++i)
	{
		std::vector<unsigned int> indexs{ faces_[i].first, faces_[i].second, faces_[i].third };
		for (int k = 0; k < indexs.size(); ++k)
		{
			auto zdiff = plane_.calcZDiff(this->_points[indexs[k]]);
			auto color = (zdiff - minDiffZ) / DIFFZ;
			if (color < 0.1)
			{
				color = 0.0f;
			}
			else if (color > 0.9)
			{
				color = 1.0f;
			}
			else
			{
				color = (color - 0.1f) / 0.8f;
			}
			this->_colors[indexs[k]] = glm::vec3(color, color, color);
		}
	}
}

PlaneFit::Plane PlaneFit::calcFitPlane(const std::vector<MeshFace>& faces_)
{
	/* 参考链接：http://blog.csdn.net/zang141588761/article/details/50523129 */
	double xx = 0, xy = 0, xz = 0;
	double yy = 0, yz = 0;
	double x = 0, y = 0, z = 0;
	double count = 0;

	for (int i = 0; i < faces_.size(); ++i)
	{
		std::vector<unsigned int> indexs{faces_[i].first, faces_[i].second, 
			faces_[i].third};
		for (int k = 0; k < indexs.size(); ++k)
		{
			auto point = _points[indexs[k]];
			xx += point.x * point.x;
			xy += point.x * point.y;
			xz += point.x * point.z;
			yy += point.y * point.y;
			yz += point.y * point.z;
			x += point.x;
			y += point.y;
			z += point.z;
			count++;
		}
	}

	//对称矩阵
	glm::mat3 matrix(0.0f);
	matrix[0][0] = xx;
	matrix[1][1] = yy;
	matrix[2][2] = count;	//顶点个数，直接写_points.size()存在问题
	matrix[0][1] = matrix[1][0] = xy;
	matrix[0][2] = matrix[2][0] = x;
	matrix[1][2] = matrix[2][1] = y;

	if (std::abs(glm::determinant(matrix)) < 10e-6)
	{
		std::cout << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << "矩阵无法求逆" << std::endl;
		throw 1;
	}

	//结果列向量
	glm::vec3 colomVec3(0.0f);
	colomVec3.x = xz;
	colomVec3.y = yz;
	colomVec3.z = z;

	auto invMatrix = glm::inverse(matrix);
	glm::vec3 vec = invMatrix * colomVec3;

	return Plane(vec.x, vec.y, vec.z);
}
