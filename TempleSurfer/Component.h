#pragma once
#include <glm/glm.hpp>

class GameObject;

class Component
{
protected:
	GameObject* gameObject;
public:
	Component();
	~Component();

	virtual void update(float elapsedTime) {};
	virtual void update(float elapsedTime, const glm::vec3& parentMatrix) {};

	inline void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; };
};

