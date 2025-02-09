#include "AttackCommand.h"
#include <iostream>

AttackCommand::AttackCommand()
{
}

void AttackCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	//make these line of code exist after the animation
	PlayerData* player = parent.getComponent<PlayerData>(DataType::PLAYER);
	if(player->curr_state != PlayerData::State::damage)
		player->curr_state = PlayerData::State::attack;
}

void AttackCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
}
