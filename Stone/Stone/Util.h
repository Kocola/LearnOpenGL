#ifndef					__UTIL_H__
#define					__UTIL_H__

/*	���ܺ���		---	2017.6.20
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

	//�����ײ��AABB��
	bool checkCollision(const GameObject& one_, const GameObject& two_);

	//�����ײ
	Collision checkCollision(const BallObject& one_, const GameObject& two_);

	//�����ײ2
	Collision checkCollision2(const BallObject& one_, const GameObject& two_);

	//���������ķ���
	Direction vectorDirection(const glm::vec2& target_);
};

#endif