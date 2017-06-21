#include <fstream>
#include <sstream>

#include "GameLevel.h"
#include "ResourceManager.h"

GameLevel::GameLevel()
{

}

GameLevel::~GameLevel()
{

}

void GameLevel::load(const std::string& path_, int levelWidth_, int levelHeight_)
{
	//销毁旧数据
	this->_bricks.clear();

	//加载文件
	std::fstream fstream(path_.c_str());
	std::vector<std::vector<int>> tileData;
	if (fstream)
	{
		std::string line;
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<int> row;
			int tileCode;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth_, levelHeight_);
		}
	}
}

void GameLevel::init(const std::vector<std::vector<int>>& tileData_, int levelWidth_, int levelHeight_)
{
	//计算维度
	int height = tileData_.size();
	int width = tileData_.front().size();
	float unitWidth = levelWidth_ / static_cast<float>(width);
	float unitHeight = levelHeight_ / static_cast<float>(height);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			//初始化位置和尺寸
			glm::vec2 pos(unitWidth * x, unitHeight * y);
			glm::vec2 size(unitWidth, unitHeight);
			if (tileData_[y][x] == 1)	//solid block
			{
				auto texture = ResourceManager::getInstance().getTexture2D("solidBlock");
				auto tmp = GameObject::create(pos, size, glm::vec3(0.8f, 0.8f, 0.7f), texture, true);
				this->_bricks.push_back(tmp);
			}
			else if (tileData_[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData_[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData_[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData_[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData_[y][x] == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}
				auto texture = ResourceManager::getInstance().getTexture2D("block");
				auto tmp = GameObject::create(pos, size, color, texture);
				this->_bricks.push_back(tmp);
			}
		}
	}
}

void GameLevel::draw()
{
	for (int i = 0; i < _bricks.size(); ++i)
	{
		if (_bricks[i].isDestroyed() == false)
		{
			_bricks[i].draw();
		}
	}
}

bool GameLevel::isCompleted() const
{
	for (int i = 0; i < _bricks.size(); ++i)
	{
		if (_bricks[i].isSolid() == false && _bricks[i].isDestroyed() == false)
		{
			return false;
		}
	}
	return true;
}

