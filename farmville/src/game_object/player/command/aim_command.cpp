#include "aim_command.h"
#include <iostream>
#include <unordered_set>

#include "core/collision_engine/collision_engine.h"

AimCommand::AimCommand()
{
}

void AimCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	GameObject* bars = scene.get_child("bars");

	if (parent.hasComponent(DataType::PLAYER)) {
		PlayerData* player = parent.getComponent<PlayerData>(DataType::PLAYER);
		if (player->target_name == "") {
			GameObject* go = CollisionEngine::getOneClosestGameObjectWithinRadius(parent.getComponent<Transform>(DataType::TRANSFORM), 100, -100, 100, Collision_object::real);
			if (go) {
				player->target_transform = go->getComponent<Transform>(DataType::TRANSFORM);
				player->target_name = go->getName();

				player->isAiming = true;
				player->curr_state = PlayerData::State::charge;

				if (!bars)
					return;

				if (bars->hasComponent(DataType::BARS))
					bars->getComponent<BarsData>(DataType::BARS)->show_bar = true;
			}
		}
	}
}

void AimCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
}

void AimCommand::show_bar()
{
}

void AimCommand::hide_bar()
{
}
