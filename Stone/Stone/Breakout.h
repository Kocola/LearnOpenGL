#ifndef					__BREAKOUT_H__
#define					__BREAKOUT_H__

/*	Breakout游戏	---	2017.6.19
*/

#include "Application.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
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
	void doCollision();

	void resetLevel();

	void resetPlayer();

private:
	GameState									_state;

	std::vector<GameLevel>				_levels;
	int												_curLevel;
	Sprite											_bgSprite;
	GameObject									_player;
	BallObject										_ball;
	ParticleGenerator							_particles;
	PostProcessor								_effect;	//后期效果
};

#endif