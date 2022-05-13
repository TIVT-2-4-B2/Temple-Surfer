#pragma once

#include "GameObject.h"
#include "GameChunk.h"
#include <vector>
#include <memory>

class GameScene
{
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<GameChunk>> gameChunks;
public:
	GameScene();
	~GameScene();

	void update(float elapsedTime);
	void draw();

	void addGameObject(std::shared_ptr<GameObject> &object);
	void removeGameObject(const std::shared_ptr<GameObject> &object);

	void addGameChunk(std::shared_ptr<GameChunk> &chunk);
	void removeGameChunk(const std::shared_ptr<GameChunk> &chunk);
};

