#include "CollisionComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include <Vector>
#include <memory>
#include <iostream>

extern bool isPlaying;
extern std::shared_ptr<GameObject> player;

CollisionComponent::CollisionComponent(glm::vec3 hitbox) : hitbox(hitbox)
{
#ifdef COLLISION_DEBUG
	glm::vec3 p(0, 0, 0); //Offset
	glm::vec4 color(0.5f, 0.5f, 0.5f, 0.5f); //Grey somewhat transparant.

	//bottom
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, -hitbox.z), color, glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, -hitbox.z), color, glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, hitbox.y, hitbox.z), color, glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(hitbox.x, -hitbox.y, hitbox.z), color, glm::vec2(0, 1), glm::vec3(0, 0, 1)));
#endif
}


CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix)
{
	if (this->gameObject->position == playerPosition && hitbox == playerHitbox)
	{
		return;
	}
	glm::vec3 gamePos = parentMatrix + this->gameObject->position;
	glm::vec3 posDif = makePositive(gamePos - playerPosition);
	glm::vec3 hitBoxSum = makePositive((hitbox / this->gameObject->scale) + playerHitbox);
	if (posDif.x <= hitBoxSum.x && posDif.y <= hitBoxSum.y && posDif.z <= hitBoxSum.z)
	{
		isPlaying = false;
	}
}

glm::vec3 CollisionComponent::makePositive(glm::vec3 vector)
{
	if (vector.x < 0)
	{
		vector.x = -vector.x;
	}
	if (vector.y < 0)
	{
		vector.y = -vector.y;
	}
	if (vector.z < 0)
	{
		vector.z = -vector.z;
	}
	return vector;
}

void CollisionComponent::update(float elapsedTime, const glm::vec3& parentMatrix)
{
	if (player != nullptr && player->getComponent<CollisionComponent>() != nullptr)
	{
		intersect(player->getComponent<CollisionComponent>()->hitbox, player->position, parentMatrix);
	}
}

void CollisionComponent::draw()
{
#ifdef COLLISION_DEBUG
	tigl::drawVertices(GL_QUADS, verts);
	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);
#endif
}
