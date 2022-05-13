#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::draw()
{
	for (auto &c : gameChunks)	c->draw();
	for (auto &c : gameObjects)	c->draw();
}

void GameScene::update(float elapsedTime)
{
	for (auto &c : gameChunks)	c->update(elapsedTime);
	for (auto &c : gameObjects) c->update(elapsedTime);
}
