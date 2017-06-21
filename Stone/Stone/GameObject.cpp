#include "GameObject.h"

#include "ResourceManager.h"

GameObject::GameObject()
	: GameObject(Sprite())
{
}

GameObject::GameObject(const Sprite& sprite_, const glm::vec2& velocity_ /*= glm::vec2(0.0f)*/)
	: _sprite(sprite_), _velocity(velocity_)
{
	this->_isDestroyed = false;
	this->_isSolid = false;
}

GameObject::~GameObject()
{

}

void GameObject::setSprite(const Sprite& sprite_)
{
	this->_sprite = sprite_;
}

void GameObject::setVecolity(const glm::vec2& velocity_)
{
	this->_velocity = velocity_;
}

void GameObject::setIsSolid(bool isSolid_)
{
	this->_isSolid = isSolid_;
}

void GameObject::setIsDestroyed(bool isDestroyed_)
{
	this->_isDestroyed = isDestroyed_;
}

void GameObject::setPosition(const glm::vec2& position_)
{
	this->_sprite.setPosition(position_);
}

void GameObject::setContentSize(const glm::vec2& size_)
{
	this->_sprite.setContentSize(size_);
}

bool GameObject::isDestroyed() const
{
	return this->_isDestroyed;
}

bool GameObject::isSolid() const
{
	return this->_isSolid;
}

glm::vec2 GameObject::getVelocity() const
{
	return this->_velocity;
}

void GameObject::draw()
{
	this->_sprite.draw();
}

GameObject GameObject::create(const glm::vec2& pos_, const glm::vec2& size_, 
	const glm::vec3& color, const Texture2D& texture_, bool isSolid_ /*= false*/)
{
	//初始化精灵
	Sprite sprite(texture_);
	sprite.setPosition(pos_);
	sprite.setContentSize(size_);
	sprite.setColor(color);

	//初始化游戏对象
	GameObject obj;
	obj.setSprite(sprite);
	obj.setIsSolid(isSolid_);

	return obj;
}

glm::vec2 GameObject::getPosition() const
{
	return this->_sprite.getPosition();
}

glm::vec2 GameObject::getContentSize() const
{
	return _sprite.getContentSize();
}
