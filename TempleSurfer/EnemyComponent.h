#pragma once


#include "Component.h"

class EnemyComponent : public Component
{
public:
	EnemyComponent();
	~EnemyComponent();

	virtual void update(float elapsedTime) override;

	
};

