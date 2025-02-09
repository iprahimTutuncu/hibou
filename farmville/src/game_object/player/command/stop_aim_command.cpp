#include "StopAimCommand.h"
#include <iostream>
#include <core/scene.h>

StopAimCommand::StopAimCommand()
{
}

void StopAimCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	GameObject* bars = scene.get_child("bars");

	PlayerData* player = parent.getComponent<PlayerData>(DataType::PLAYER);
	if (player->target_name != "")
		player->target_name = "";

	player->isAiming = false;


	if (!bars)
		return;

	if (bars->hasComponent(DataType::BARS))
		bars->getComponent<BarsData>(DataType::BARS)->show_bar = false;
}

void StopAimCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{

}
