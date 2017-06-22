#ifndef					__BREAKOUT_H__
#define					__BREAKOUT_H__

/*	Breakout游戏	---	2017.6.19
*/

#include "Application.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "Powerup.h"
#include "Sprite.h"

class Breakout : public Application
{
	enum GameState
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};
public:
	Breakout();

	~Breakout();

private:
	void init();

	void initShaderProgram();

protected:
	virtual void processInput(GLfloat dt) override;

	virtual void update(GLfloat dt_) override;

public:
	virtual void draw() override;

private:
	void spawnPowerups(const GameObject& block_);		//2017.6.22

	void updatePowerups(GLfloat dt_);		//2017.6.22

	bool shouldSpwan(int chance_);		//2017.6.22

	void activatePowerup(const Powerup& powerup_);	//2017.6.22

	bool isOtherPowerupActive(const std::vector<Powerup>& powerups_, const std::string& type_);		//2017.6.22

private:
	void doCollision();

private:
	void reset();

	void resetLevel();

	void resetPlayer();

	void resetEffect();

private:
	GameState									_state;

	std::vector<GameLevel>				_levels;
	int												_curLevel;
	Sprite											_bgSprite;
	GameObject									_player;
	BallObject										_ball;
	ParticleGenerator							_particles;
	PostProcessor								_effect;	//后期效果

	std::vector<Powerup>					_powerups;
};

#endif