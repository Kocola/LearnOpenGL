#include "BallObject.h"

BallObject::BallObject()
	: _radius(12.5f), _stuck(true)
{
}

BallObject::BallObject(const glm::vec2& pos_, float radius_, const glm::vec2& velocity_, Texture2D texture_)
{
	this->setPosition(pos_);
	this->setContentSize(glm::vec2(radius_ * 2.0f));
	this->_sprite.setTexture(texture_);
	this->setVecolity(velocity_);
	this->_radius = radius_;
	this->_stuck = true;
}

BallObject::~BallObject()
{

}

glm::vec2 BallObject::move(float dt_, int windowWidth_)
{
	//没有和玩家board碰撞
	if (_stuck == false)
	{
		auto lastPosition = this->getPosition();
		auto lastVelocity = this->getVelocity();
		auto size = this->getContentSize();
		auto curPosition = lastPosition + lastVelocity * dt_;
		auto curVelocity = lastVelocity;
		if (curPosition.x <= 0.0f)
		{
			curVelocity.x = -lastVelocity.x;
			curPosition.x = 0.0f;
		}
		else if (curPosition.x + size.x >= windowWidth_)
		{
			curVelocity.x = -lastVelocity.x;
			curPosition.x = windowWidth_ - size.x;
		}
		if (curPosition.y <= 0.0f)
		{
			curVelocity.y = -lastVelocity.y;
			curPosition.y = 0.0f;
		}
		this->setPosition(curPosition);
		this->setVecolity(curVelocity);
	}
	return this->getPosition();
}

void BallObject::reset(const glm::vec2& position_, const glm::vec2& velocity_)
{
	this->setPosition(position_);
	this->setVecolity(velocity_);
	this->_stuck = true;	//初始时是在玩家board上
}

void BallObject::setStuck(bool stuck_)
{
	this->_stuck = stuck_;
}

bool BallObject::isStuck() const
{
	return this->_stuck;
}

float BallObject::getRadius() const
{
	return this->_radius;
}

