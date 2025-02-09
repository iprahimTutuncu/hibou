#include "action_command.h"
#include "../../../states/factory/UIFactory.h"
#include <thsan/ressourceManager/StateManager.h>

ActionCommand::ActionCommand()
{
}

void ActionCommand::execute(GameObject& parent, Scene& scene, const float& dt)
{
	std::shared_ptr<State> current = RessourceManager::StateManager::get<State>(scene.getParent()->getId());
	UIFactory uiFactory(current);

	std::shared_ptr<State> playerlistChoiceBox = uiFactory.createPlayerListChoiceBox(parent, scene);
	current->getParent()->changeState(playerlistChoiceBox, true);
}

void ActionCommand::undo(GameObject& parent, Scene& scene, const float& dt)
{
}
