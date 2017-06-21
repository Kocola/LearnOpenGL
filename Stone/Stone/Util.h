#ifndef					__UTIL_H__
#define					__UTIL_H__

/*	功能函数		---	2017.6.20
*/

#include "Common.h"

class BallObject;
class GameObject;

namespace Util
{
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	typedef std::tuple<bool, Direction, glm::vec2> Collision;

	//检测碰撞（AABB）
	bool checkCollision(const GameObject& one_, const GameObject& two_);

	//检测碰撞
	Collision checkCollision(const BallObject& one_, const GameObject& two_);

	//检测碰撞2
	Collision checkCollision2(const BallObject& one_, const GameObject& two_);

	//计算向量的方向
	Direction vectorDirection(const glm::vec2& target_);
};

#endif