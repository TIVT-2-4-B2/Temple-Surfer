#include "GameScene.h"
#include "ChunkGenerator.h"
#include "FloorComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ChunkGenerator chunkGenerator = ChunkGenerator();

extern int score = 0;

GameScene::GameScene()
{
	chunkGenerator.generatorInit();
}

GameScene::~GameScene()
{
}

void GameScene::draw()
{	
	for (auto& c : gameObjects)	c->draw();
	for (int i = gameChunks.size() - 1; i >= 0; i--) {
		gameChunks[i]->draw();
	}
}

void GameScene::addGameObject(std::shared_ptr<GameObject>& object)
{
	gameObjects.push_back(object);
}

void GameScene::removeGameObject(const std::shared_ptr<GameObject>& object)
{
	for (auto& pointer : gameObjects)
	{
		if (pointer == object)
		{
			pointer = nullptr;
		}
	}
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), nullptr), gameObjects.end());
}

void GameScene::addGameChunk(std::shared_ptr<GameChunk> chunk)
{
	gameChunks.push_back(chunk);
}

void GameScene::removeGameChunk(const std::shared_ptr<GameChunk>& chunk)
{
	for (auto& pointer : gameChunks)
	{
		if (pointer == chunk)
		{
			pointer = nullptr;
		}
	}
	gameChunks.erase(std::remove(gameChunks.begin(), gameChunks.end(), nullptr), gameChunks.end());
}

void GameScene::update(float elapsedTime)
{
	for (auto& chunk : gameChunks) {

		chunk->moveChunk(chunk->gamePosition += glm::vec3(0, 0, 2 * elapsedTime));
		chunk->update(elapsedTime);

		//std::cout << chunk->gamePosition.z << std::endl;
		if (chunk->gamePosition.z > -(FLOOR_LENGTH-Z_THRESHOLD)) {

			const int scoreIncrementValue = 10;
			if (!scoreReset && std::abs(((int)chunk->gamePosition.z) % scoreIncrementValue) == 0)
			{
				scoreReset = true;
				score++;
				std::cout << score << std::endl;
			}
			else if (scoreReset && (std::abs(((int)chunk->gamePosition.z) % scoreIncrementValue) != 0))
			{
				scoreReset = false;
			}
		}

		if (chunk->gamePosition.z > Z_THRESHOLD)
		{
			removeGameChunk(chunk);
			addGameChunk(chunkGenerator.getChunk());
		}
	}
	for (auto& c : gameObjects) c->update(elapsedTime);
}
