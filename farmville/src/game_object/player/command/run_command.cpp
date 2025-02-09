#include "run_command.h"
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

RunCommand::RunCommand()
{
}

void RunCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	PlayerData* playerData{nullptr};
	if (parent.hasComponent(DataType::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(DataType::PLAYER);
		if(playerData->curr_state == PlayerData::State::walk)
			playerData->curr_state = PlayerData::State::run;
	}
}

void RunCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasComponent(DataType::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(DataType::PLAYER);
		if (playerData->curr_state == PlayerData::State::run)
			playerData->curr_state = PlayerData::State::walk;
	}
}
