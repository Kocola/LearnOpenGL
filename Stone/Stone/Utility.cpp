#include "Utility.h"

glm::vec3 rotate(const glm::vec3& u_, float angle_, const glm::vec3& v_)
{
	using namespace glm;

	auto tmp = glm::normalize(v_);

	float AngleRad = glm::radians(angle_);

	float Sin = sin(AngleRad * float(0.5));
	float Cos = cos(AngleRad * float(0.5));

	glm::quat q = glm::quat(Cos, u_ * float(Sin));
	glm::quat cq = glm::conjugate(q);

	auto result = q * glm::quat(0, v_) * cq;
	return glm::vec3(result.x, result.y, result.z);
}

glm::mat2x3 operator*(const glm::mat2x2& mat22_, const glm::mat2x3& mat23_)
{
	glm::mat3x2 mat32 = glm::transpose(mat23_);
	glm::mat2x3 result;
	result[0][0] = glm::dot(mat22_[0], mat32[0]);
	result[0][1] = glm::dot(mat22_[0], mat32[1]);
	result[0][2] = glm::dot(mat22_[0], mat32[2]);
	result[1][0] = glm::dot(mat22_[1], mat32[0]);
	result[1][1] = glm::dot(mat22_[1], mat32[1]);
	result[1][2] = glm::dot(mat22_[1], mat32[2]);
	return result;
}