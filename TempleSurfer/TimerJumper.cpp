#include "TimerJumper.h"
#include "GameObject.h"
#include "MoveToComponent.h"



TimerJumper::TimerJumper()
{
}


TimerJumper::~TimerJumper()
{
}

void TimerJumper::update(float elapsedTime)
{
	timer -= elapsedTime;
	if (timer <= 0)
	{
		MoveToComponent* moveTo = gameObject->getComponent<MoveToComponent>();
		moveTo->target.x = targetX;
		targetX = -targetX;

		timer = 5;
	}

}
