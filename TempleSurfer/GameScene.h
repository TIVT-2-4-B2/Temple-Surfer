#pragma once

#include "GameObject.h"
#include "GameChunk.h"
#include <list>
#include <memory>

class GameScene
{
	DrawComponent* drawComponent = nullptr;
	std::list<std::shared_ptr<GameObject>> gameObjects;
	std::list<std::shared_ptr<GameChunk>> gameChunks;
public:
	GameScene();
	~GameScene();

	void update(float elapsedTime);
	void draw();

	void addGameObject(GameObject* object);
	template<class T>
	T* getGameObject()
	{
		for (auto c : gameObjects)
		{
			T* t = dynamic_cast<T*>(c);
			if (t) return t;
		}
		return nullptr;
	}

	template<class T>
	void removeGameObject()
	{
		components.remove_if([](GameObject* c)
			{
				T* t = dynamic_cast<T*>(c);
				return t != nullptr;
			});
	}

	void addGameChunk(GameChunk* object);
	template<class T>
	T* getGameChunk()
	{
		for (auto c : gameChunk)
		{
			T* t = dynamic_cast<T*>(c);
			if (t) return t;
		}
		return nullptr;
	}

	template<class T>
	void removeGameChunk()
	{
		components.remove_if([](GameChunk* c)
			{
				T* t = dynamic_cast<T*>(c);
				return t != nullptr;
			});
	}
};

