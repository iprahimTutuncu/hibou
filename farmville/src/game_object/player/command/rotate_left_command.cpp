#include "rotate_left_command.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

RotateLeftCommand::RotateLeftCommand()
{
	//ctor
}

RotateLeftCommand::~RotateLeftCommand()
{
	//dtor
}

void RotateLeftCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getComponent<CameraData>(DataType::CAMERA);
	PlayerData* player_data = parent.getComponent<PlayerData>(DataType::PLAYER);

	if (player_data->target_name == "")
		camera_data->angle += dt * 3.f;
}

void RotateLeftCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getComponent<CameraData>(DataType::CAMERA);
	PlayerData* player_data = parent.getComponent<PlayerData>(DataType::PLAYER);

	if (player_data->target_name == "")
		camera_data->angle -= dt * 3.f;
}

