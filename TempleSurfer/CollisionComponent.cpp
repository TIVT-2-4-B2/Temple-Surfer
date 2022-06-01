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
    glm::vec3 hitBoxSum = makePositive(hitbox + playerHitbox);
    if (posDif.x <= (0.5 * hitBoxSum.x) && posDif.y <= (0.5 * hitBoxSum.y) && posDif.z <= (0.5 * hitBoxSum.z))
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
