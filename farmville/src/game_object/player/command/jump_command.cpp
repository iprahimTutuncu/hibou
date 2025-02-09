#include "jump_command.h"
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <core/scene.h>

JumpCommand::JumpCommand()
{
}

void JumpCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	if (!parent.hasComponent(DataType::PHYSIC_BODY))
		return;

	PhysicBodyData* phy = parent.getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);

	if (phy->onGround) {
		phy->isJump = true;
		phy->onGround = false;
	}
	else {
		phy->isDoubleJump = true;
	}

}

void JumpCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
}
