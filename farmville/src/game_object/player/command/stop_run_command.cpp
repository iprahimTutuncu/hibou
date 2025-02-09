#include "stop_run_command.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

StopRunCommand::StopRunCommand()
{
}

void StopRunCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasComponent(DataType::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(DataType::PLAYER);
		if (playerData->curr_state == PlayerData::State::run)
			playerData->curr_state = PlayerData::State::walk;
	}
}

void StopRunCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasComponent(DataType::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(DataType::PLAYER);
		if (playerData->curr_state == PlayerData::State::walk)
			playerData->curr_state = PlayerData::State::run;
	}
}
