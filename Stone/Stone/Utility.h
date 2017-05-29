#ifndef					__UTILITY_H__
#define					__UTILITY_H__

#include "Common.h"

//报错：C2783
// template<typename T>
// glm::detail::tvec3<T> rotate(const glm::detail::tquat<T>& u_,
// 	typename glm::detail::tvec3<T>::value_type const& angle_,
// 	const glm::detail::tvec3<T>& v_)
// {
// 	using namespace glm;
// 
// 	auto tmp = glm::normalize(v_);
// 
// #ifdef GLM_FORCE_RADIANS
// 	typename detail::tquat<T>::value_type const AngleRad(angle);
// #else
// 	typename detail::tvec3<T>::value_type const AngleRad = radians(angle);
// #endif
// 	typename detail::tvec3<T>::value_type const Sin = sin(AngleRad * T(0.5));
// 	typename detail::tvec3<T>::value_type const Cos = cos(AngleRad * T(0.5));
// 
// 	detail::tquat<T> q = detail::tquat<T>(Cos, u_ * T(Sin));
// 	detail::tquat<T> cq = glm::conjugate(q);
// 
// 	return q * v_ * cq;
// }

//u：旋转轴；angle：旋转角度；v：被旋转向量
glm::vec3 rotate(const glm::vec3& u_, float angle_, const glm::vec3& v_);

glm::mat2x3 operator*(const glm::mat2x2& mat22_, const glm::mat2x3& mat23_);

#endif