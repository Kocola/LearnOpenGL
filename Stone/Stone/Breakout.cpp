#include "Breakout.h"

#include <algorithm>

#include "InputManager.h"
#include "ResourceManager.h"
#include "Util.h"

const glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(100.0f, -350.f);

const float BALL_RADIUS = 12.5f;

const std::string SPEED_TYPE = "speed";
const std::string STICKY_TYPE = "sticky";
const std::string PASS_THROUGH_TYPE = "pass-throught";
const std::string INCREASE_TYPE = "pad-size-increase";
const std::string CONFUSE_TYPE = "confuse";
const std::string CHAOS_TYPE = "chaos";

const int PROFIT_POWERUP_ODDS = 75;
const int NEGATIVE_POWERUP_ODDS = 30;

Breakout::Breakout()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout")
{
	init();
}

Breakout::~Breakout()
{
}

void Breakout::init()
{
	//���عؿ�
	std::vector<std::string> levelsName{"one.lvl", "two.lvl", "three.lvl", "four.lvl"};
	for (int i = 0; i < levelsName.size(); ++i)
	{
		GameLevel tmp;
		tmp.load(std::string("resources/breakout/levels/") + levelsName[i], SCREEN_WIDTH,
			SCREEN_HEIGHT * 0.5f);
		this->_levels.push_back(tmp);
	}
	//���ó�ʼ�ؿ�Ϊ1
	this->_curLevel = 0;

	//��������
	_bgSprite = Sprite(ResourceManager::getInstance().getTexture2D("background"));
	_bgSprite.setContentSize(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

	//��Ҷ���
	glm::vec2 playerSize = glm::vec2(100, 20);
	glm::vec2 playerPosition = glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT)
		- glm::vec2(playerSize.x / 2.0f, playerSize.y);
	_player = GameObject::create(playerPosition, playerSize, glm::vec3(1.0f),
		ResourceManager::getInstance().getTexture2D("paddle"));
	_player.setVecolity(glm::vec2(45.0f, 0.0f));	//��������ֱ�ƶ�

	//С��
	glm::vec2 ballPos = _player.getPosition() + glm::vec2(_player.getContentSize().x / 2.0f - BALL_RADIUS,
		-BALL_RADIUS * 2.0f);
	_ball = BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::getInstance().getTexture2D("awesomeface"));

	//��ʼ������������
	_particles = ParticleGenerator(500);

	//��ʼ�����ڴ���Ч��
	_effect = PostProcessor(SCREEN_WIDTH, SCREEN_HEIGHT);
}


void Breakout::processInput(GLfloat dt)
{
	if (this->_state == GAME_ACTIVE)
	{
		GLfloat velocity = _player.getVelocity().x;
		float newX = _player.getPosition().x;
		if (InputManager::getInstance().isKeyPressed(GLFW_KEY_A))
		{
			newX = _player.getPosition().x - velocity;
			newX = newX < 0.0f ? 0.0f : newX;

			auto delta = glm::vec2(newX - _player.getPosition().x, 0.0f);
			_player.setPosition(_player.getPosition() + delta);
			if (_ball.isStuck() == true)
			{
				_ball.setPosition(_ball.getPosition() + delta);
			}
		}

		if (InputManager::getInstance().isKeyPressed(GLFW_KEY_D))
		{
			newX = _player.getPosition().x + velocity;
			if (newX > SCREEN_WIDTH - _player.getContentSize().x)
			{
				newX = SCREEN_WIDTH - _player.getContentSize().x;
			}

			auto delta = glm::vec2(newX - _player.getPosition().x, 0.0f);
			_player.setPosition(_player.getPosition() + delta);
			if (_ball.isStuck() == true)
			{
				_ball.setPosition(_ball.getPosition() + delta);
			}
		}

		if (InputManager::getInstance().isKeyPressed(GLFW_KEY_S))
		{
			this->_ball.setStuck(false);
		}
	}
}

void Breakout::update(GLfloat dt_)
{
	//ÿһ֡������С���λ��
	_ball.move(dt_, SCREEN_WIDTH);
	//�����ײ
	this->doCollision();
	//����������Ϣ
	_particles.update(dt_, _ball, 6, glm::vec2(_ball.getRadius() / 2.0f));

	if (_ball.getPosition().y >= SCREEN_HEIGHT)
	{
		//this->resetLevel();
		//this->resetPlayer();
		//init();
		//������
		reset();
	}

	//��ײЧ��
	if (_effect.getShakeTime() > 0.0f)
	{
		_effect.setShakeTime(_effect.getShakeTime() - dt_);
		if (_effect.getShakeTime() <= 0.0f)
		{
			_effect.setIsShake(false);
		}
	}

	//���µ�����Ϣ
	updatePowerups(dt_);
}

void Breakout::draw()
{
	//�ر���Ȳ��ԣ���Ȼ����Z-FIGHTING
	glDisable(GL_DEPTH_TEST);

	if (this->_state == GAME_ACTIVE)
	{
		_effect.startRender();

		//���Ʊ���
		this->_bgSprite.draw();
		//���ƹؿ���Ϣ
		this->_levels[_curLevel].draw();
		//������Ҷ���
		this->_player.draw();
		//����С��
		this->_ball.draw();
		//��������
		this->_particles.draw();
		//���Ƶ���
		for (auto& powerup : _powerups)
		{
			if (powerup.isDestroyed() == false)
			{
				powerup.draw();
			}
		}

		_effect.endRender();

		_effect.render();
	}

	glEnable(GL_DEPTH_TEST);
}

void Breakout::spawnPowerups(const GameObject& block_)
{
	Texture2D texture;
	glm::vec2 position = block_.getPosition();
	if (shouldSpwan(PROFIT_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupSpeed");
		this->_powerups.push_back(Powerup(
			SPEED_TYPE, position, texture, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f));
	}

	if (shouldSpwan(PROFIT_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupSticky");
		this->_powerups.push_back(Powerup(
			STICKY_TYPE, position, texture, glm::vec3(1.0f, 0.5f, 1.0f), 20.f));
	}

	if (shouldSpwan(PROFIT_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupPassThrough");
		this->_powerups.push_back(Powerup(
			PASS_THROUGH_TYPE, position, texture, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f));
	}

	if (shouldSpwan(PROFIT_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupIncrease");
		this->_powerups.push_back(Powerup(
			INCREASE_TYPE, position, texture, glm::vec3(1.0f, 0.6f, 0.4f), 0.0f));
	}

	if (shouldSpwan(NEGATIVE_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupConfuse");
		this->_powerups.push_back(Powerup(
			CONFUSE_TYPE, position, texture, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f));
	}

	if (shouldSpwan(NEGATIVE_POWERUP_ODDS))
	{
		texture = ResourceManager::getInstance().getTexture2D("powerupChaos");
		this->_powerups.push_back(Powerup(
			CHAOS_TYPE, position, texture, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f));
	}
}

void Breakout::updatePowerups(GLfloat dt_)
{
	for (auto& powerup : this->_powerups)
	{
		//���µ���λ��
		powerup.setPosition(powerup.getPosition() + powerup.getVelocity() * dt_);
		if (powerup.isActivated())
		{
			//���µ��߳���ʱ��
			powerup.setDuration(powerup.getDuration() - dt_);
			if (powerup.getDuration() <= 0.0f)
			{
				//���øõ���ʧЧ
				powerup.setActivated(false);

				const std::string type = powerup.getType();
				//ɾ���õ��ߵ�Ч��
				if (type == STICKY_TYPE)
				{
					if (isOtherPowerupActive(_powerups, type) == false)
					{
						//û��������ͬ�ĵ���Ч�����ڼ���ʱ������
						_ball.setSticky(false);
						_player.setColor(glm::vec3(1.0f));
					}
				}
				else if (type == PASS_THROUGH_TYPE)
				{
					if (isOtherPowerupActive(_powerups, type) == false)
					{
						_ball.setPassThrough(false);
						_player.setColor(glm::vec3(1.0f));
					}
				}
				else if (type == CONFUSE_TYPE)
				{
					if (isOtherPowerupActive(_powerups, type) == false)
					{
						_effect.setConfuse(false);
					}
				}
				else if (type == CHAOS_TYPE)
				{
					if (isOtherPowerupActive(_powerups, type) == false)
					{
						_effect.setChaos(false);
					}
				}
			}
		}
	}
	//�ӵ���������ɾ��ʧЧ�Ҵ��ڴ�����״̬�ĵ���
	this->_powerups.erase(
		std::remove_if(this->_powerups.begin(), this->_powerups.end(), 
		[](const Powerup& powerup_){
			return powerup_.isDestroyed() && powerup_.isActivated() == false;
		}
	), this->_powerups.end());
}

bool Breakout::shouldSpwan(int chance_)
{
	int random = rand() % chance_;
	return random == 0;
}

void Breakout::activatePowerup(const Powerup& powerup_)
{
	std::string type = powerup_.getType();
	if (type == SPEED_TYPE)
	{
		_ball.setVecolity(_ball.getVelocity() * 1.2f);
	}
	else if (type == STICKY_TYPE)
	{
		_ball.setSticky(true);
		_player.setColor(glm::vec3(1.0f, 0.5f, 1.0f));
	}
	else if (type == PASS_THROUGH_TYPE)
	{
		_ball.setPassThrough(true);
		_ball.setColor(glm::vec3(1.0f, 0.5f, 0.5f));
	}
	else if (type == CONFUSE_TYPE)
	{
		if (_effect.isConfuse() == false)
		{
			_effect.setConfuse(true);
		}
	}
	else if (type == CHAOS_TYPE)
	{
		if (_effect.isChaos() == false)
		{
			_effect.setChaos(true);
		}
	}
}

bool Breakout::isOtherPowerupActive(const std::vector<Powerup>& powerups_, 
	const std::string& type_)
{
	for (auto& powerup : powerups_)
	{
		if (powerup.isActivated())
		{
			if (powerup.getType() == type_)
			{
				return true;
			}
		}
	}
	return false;
}

void Breakout::doCollision()
{
	for (GameObject& box : _levels[_curLevel]._bricks)
	{
		if (box.isDestroyed() == false)
		{
			Util::Collision collision = Util::checkCollision(_ball, box);
			//������ײ
			if (std::get<0>(collision))
			{
				if (box.isSolid() == false)
				{
					box.setIsDestroyed(true);
					this->spawnPowerups(box);	//2017.6.22
				}
				else//��ײЧ��
				{
					_effect.setShakeTime(0.5f);
					_effect.setIsShake(true);
				}

				//��ײ����
				Util::Direction dir = std::get<1>(collision);
				glm::vec2 diffVector = std::get<2>(collision);
				if (!(_ball.isPassThrough() && !box.isSolid()))	//����Ƿ�solid�����Ҳ���passthrough����ô��ִ����ײЧ��
				{
					//ˮƽ��ײ
					if (dir == Util::Direction::LEFT || dir == Util::Direction::RIGHT)
					{
						_ball.setVecolity(glm::vec2(_ball.getVelocity().x * -1.0f, _ball.getVelocity().y));
						float penetration = _ball.getRadius() - std::abs(diffVector.x);
						glm::vec2 position = _ball.getPosition();
						if (dir == Util::Direction::LEFT)
						{
							position.x += penetration;
						}
						else
						{
							position.x -= penetration;
						}
						_ball.setPosition(position);
					}
					//��ֱ��ײ
					else
					{
						_ball.setVecolity(_ball.getVelocity() * glm::vec2(1.0f, -1.0f));
						float penetration = _ball.getRadius() - std::abs(diffVector.y);
						glm::vec2 position = _ball.getPosition();
						if (dir == Util::Direction::UP)
						{
							position -= penetration;
						}
						else
						{
							position += penetration;
						}
						_ball.setPosition(position);
					}
				}
			}
		}
	}

	//С������board��ײ
	Util::Collision result = Util::checkCollision(_ball, _player);
	if (_ball.isStuck() == false && std::get<0>(result))
	{
		//���С���ںδ�ײ��board��������ײ����ı��ٶ�
		float centerBoard = _player.getPosition().x + _player.getContentSize().x / 2.0f;
		float distance = (_ball.getPosition().x + _ball.getRadius()) - centerBoard;
		float percentage = distance / (_player.getContentSize().x / 2.0f);

		const float strength = 2.0f;
		glm::vec2 oldVelocity = _ball.getVelocity();
		glm::vec2 velocity = _ball.getVelocity();
		velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		velocity.y = -std::abs(oldVelocity.y);
		velocity = glm::normalize(velocity) * glm::length(oldVelocity);
		_ball.setVecolity(velocity);

		//2017.6.22
		_ball.setStuck(_ball.isSticky());
	}

	//2017.6.22
	for (auto& powerup : this->_powerups)
	{
		if (powerup.isDestroyed() == false)
		{
			if (powerup.getPosition().y >= SCREEN_HEIGHT)
			{
				powerup.setIsDestroyed(true);
			}
			if (Util::checkCollision(_player, powerup))
			{
				activatePowerup(powerup);
				powerup.setIsDestroyed(true);
				powerup.setActivated(true);
			}
		}
	}
}

void Breakout::reset()
{
	this->resetLevel();
	this->resetPlayer();
	this->resetEffect();
}

void Breakout::resetLevel()
{
	//���عؿ�
	std::vector<std::string> levelsName{ "one.lvl", "two.lvl", "three.lvl", "four.lvl" };
	for (int i = 0; i < levelsName.size(); ++i)
	{
		this->_levels[i].load(std::string("resources/breakout/levels/") + levelsName[i], SCREEN_WIDTH,
			SCREEN_HEIGHT * 0.5f);
	}
}

void Breakout::resetPlayer()
{
	//������Һ�С���״̬
	//��Ҷ���
	glm::vec2 playerSize = glm::vec2(100, 20);
	glm::vec2 playerPosition = glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT)
		- glm::vec2(playerSize.x / 2.0f, playerSize.y);
	_player.setPosition(playerPosition);
	_player.setContentSize(playerSize);
	_player.setVecolity(glm::vec2(45.0f, 0.0f));	//��������ֱ�ƶ�

	//С��
	glm::vec2 ballPos = _player.getPosition() + glm::vec2(_player.getContentSize().x / 2.0f - BALL_RADIUS,
		-BALL_RADIUS * 2.0f);
	_ball.reset(ballPos, INITIAL_BALL_VELOCITY);
}

void Breakout::resetEffect()
{
	_effect = PostProcessor(SCREEN_WIDTH, SCREEN_HEIGHT);
}
