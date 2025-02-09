#include "move_right_command.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

MoveRightCommand::MoveRightCommand()
{
    //ctor
}

MoveRightCommand::~MoveRightCommand()
{
    //dtor
}

void MoveRightCommand::execute(GameObject &parent, Scene &scene, const float& dt)
{
	const double pi = 3.14159265358979323846;

	PlayerData* player_data = parent.getComponent<PlayerData>(DataType::PLAYER);
	Transform* player_transform = parent.getComponent<Transform>(DataType::TRANSFORM);
	PhysicBodyData* phy = parent.getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getComponent<CameraData>(DataType::CAMERA)->angle;

	GameObject* target = scene.get_child(player_data->target_name);
	glm::vec3 n;

	if (player_data->target_name != "" && target) {
		if (target) {
			glm::vec3 target_position = target->getComponent<Transform>(DataType::TRANSFORM)->position;
			n = player_transform->position - target_position;
			n = -glm::normalize(glm::vec3(-n.z, 0.f, n.x));
		}
	}
	else {
		n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));
	}

	phy->direction += n;
	phy->force = 2.f;
}

void MoveRightCommand::undo(GameObject &parent, Scene &scene, const float& dt)
{
	PhysicBodyData* phy = parent.getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getComponent<CameraData>(DataType::CAMERA)->angle;

	const double pi = 3.14159265358979323846;
	glm::vec3 n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));

	phy->direction += -n;
	phy->force = 2.f;
}

