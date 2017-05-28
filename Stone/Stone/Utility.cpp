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