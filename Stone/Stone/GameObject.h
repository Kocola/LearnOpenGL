#ifndef					__GAME_OBJECT_H__
#define					__GAME_OBJECT_H__

/*	游戏对象	---	2017.6.19
*/

#include "Common.h"
#include "Sprite.h"

struct GameObject
{
public:
	GameObject();

	GameObject(const Sprite& sprite_, const glm::vec2& velocity_ = glm::vec2(0.0f));

	~GameObject();

public:
	void setSprite(const Sprite& sprite_);

	void setVecolity(const glm::vec2& velocity_);

	void setIsSolid(bool isSolid_);

	void setIsDestroyed(bool isDestroyed_);

	void setPosition(const glm::vec2& position_);

	void setContentSize(const glm::vec2& size_);

	void setColor(const glm::vec3& color_);

	void setTexture(const Texture2D& texture_);

public:
	bool isDestroyed() const;

	bool isSolid() const;

	glm::vec2 getVelocity() const;

	glm::vec2 getPosition() const;

	glm::vec2 getContentSize() const;

public:
	static GameObject create(const glm::vec2& pos_, const glm::vec2& size_, 
		const glm::vec3& color, const Texture2D& texture_, bool isSolid_ = false);

public:
	virtual void draw();

protected:
	glm::vec2				_velocity;			//速度
	GLboolean			_isSolid;		
	GLboolean			_isDestroyed;		//是否被销毁

	Sprite					_sprite;
};

#endif