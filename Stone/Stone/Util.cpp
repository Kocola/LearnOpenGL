#include "Util.h"

#include <tuple>

#include "BallObject.h"
#include "GameObject.h"

bool Util::checkCollision(const GameObject& one_, const GameObject& two_)
{
	//X轴相交？
	bool collisionX = (one_.getPosition().x + one_.getContentSize().x >= two_.getPosition().x) &&
		(two_.getPosition().x + two_.getContentSize().x >= one_.getPosition().x);
	//Y轴相交
	bool collisionY = (one_.getPosition().y + one_.getContentSize().y >= two_.getPosition().y) &&
		(two_.getPosition().y + two_.getContentSize().y >= one_.getPosition().y);

	return collisionX && collisionY;
}

Util::Collision Util::checkCollision(const BallObject& one_, const GameObject& two_)
{
	//计算圆中心
	glm::vec2 center(one_.getPosition() + one_.getRadius());
	//计算AABB信息（中心，中心至某个顶点的向量）
	glm::vec2 aabbHalfExtents(two_.getContentSize() / 2.0f);
	glm::vec2 aabbCenter(two_.getPosition() + aabbHalfExtents);
	//计算圆和长方形中心之间的向量
	glm::vec2 difference = center - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
	//计算长方形边界离圆中心最近的点P
	glm::vec2 closet = aabbCenter + clamped;
	//计算点P和圆心之间的向量
	difference = closet - center;

	if (glm::length(difference) <= one_.getRadius())
	{
		return std::make_tuple(true, vectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f));
	}
}

Util::Collision Util::checkCollision2(const BallObject& one_, const GameObject& two_)
{
	//计算圆中心
	glm::vec2 center(one_.getPosition() + one_.getRadius());
	//计算AABB信息（中心，中心至某个顶点的向量）
	glm::vec2 aabbHalfExtents(two_.getContentSize() / 2.0f);
	glm::vec2 aabbCenter(two_.getPosition() + aabbHalfExtents);
	//计算圆和长方形中心之间的向量（统一至两个维度都为正）
	glm::vec2 v = glm::max(center - aabbCenter, aabbCenter - center);
	//长方形顶点和圆中心的向量
	glm::vec2 u = glm::max(v - aabbHalfExtents, glm::vec2(0.0f));
	
	if (glm::length(u) <= one_.getRadius())
	{
		return std::make_tuple(true, vectorDirection(u), u);
	}
	else
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f));
	}
}

Util::Direction Util::vectorDirection(const glm::vec2& target_)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),		//UP
		glm::vec2(0.0f, -1.0f),		//DOWN
		glm::vec2(-1.0f, 0.0f),		//LEFT
		glm::vec2(1.0f, 0.0f)
	};

	float max = 0.0f;
	int bestMatch = -1;
	for (int i = 0; i < sizeof(compass) / sizeof(compass[0]); ++i)
	{
		float dotProduct = glm::dot(glm::normalize(target_), compass[i]);
		if (dotProduct > max)
		{
			max = dotProduct;
			bestMatch = i;
		}
	}
	return (Direction)bestMatch;
}

