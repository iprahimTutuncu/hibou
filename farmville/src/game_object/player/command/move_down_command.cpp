#include "move_down_command.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

MoveDownCommand::MoveDownCommand()
{
    //ctor
}

MoveDownCommand::~MoveDownCommand()
{
    //dtor
}

void MoveDownCommand::execute(GameObject &parent, Scene &scene, const float& dt)
{
	PhysicBodyData* phy = parent.getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);
	Transform* trans = parent.getComponent<Transform>(DataType::TRANSFORM);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if(cam)
		angle_camera = cam->getComponent<CameraData>(DataType::CAMERA)->angle;


	PlayerData* pd = parent.getComponent<PlayerData>(DataType::PLAYER);
	const double pi = 3.14159265358979323846;

	glm::vec3 n;

	if (pd->target_name != "") {
		GameObject* target = scene.get_child(pd->target_name);
		if (target) {
			glm::vec3 target_position = target->getComponent<Transform>(DataType::TRANSFORM)->position;
			n = trans->position - target_position;
			n = glm::vec3(n.x, 0.f, n.z);
			if (glm::length(n) < std::numeric_limits<float>::min())
				n = glm::vec3(0.f);
			else
				n = glm::normalize(n);
		}
	}
	else {
		n = glm::normalize(glm::vec3(sin(angle_camera), 0.f, cos(angle_camera)));
	}
	phy->direction += n;
	phy->force = 2.f;
}

void MoveDownCommand::undo(GameObject &parent, Scene &scene,  const float& dt)
{
	PlayerData* data = parent.getComponent<PlayerData>(DataType::PLAYER);
	PhysicBodyData* phy = parent.getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getComponent<CameraData>(DataType::CAMERA)->angle;

	const double pi = 3.14159265358979323846;
	glm::vec3 n = glm::normalize(glm::vec3(sin(angle_camera), 0.f, cos(angle_camera)));

	phy->direction += -n;
	phy->force = 2.f;
}
