#ifndef					__GAME_LEVEL_H__
#define					__GAME_LEVEL_H__

/*	ÓÎÏ·¹Ø¿¨	---	2017.6.19
*/

#include <vector>

#include "GameObject.h"

class Breakout;

class GameLevel
{
	friend class Breakout;
public:
	GameLevel();

	~GameLevel();

public:
	void load(const std::string& path_, int levelWidth_, int levelHeight_);

	void draw();

private:
	bool isCompleted() const;

private:
	void init(const std::vector<std::vector<int>>& tileData_, int levelWidth_,
		int levelHeight_);

private:
	std::vector<GameObject>				_bricks;
};

#endif