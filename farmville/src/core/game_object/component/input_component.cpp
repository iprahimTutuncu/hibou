#include "input_component.h"
#include "../game_object.h"
#include "input/Command.h"
#include <thsan/input/control_setting.h>

InputComponent::~InputComponent()
{
    for(Command* cmd: commandHistory)
        delete cmd;
    commandHistory.clear();
}

void InputComponent::undo(Scene& scene, const float& dt)
{
    commandHistory[currCommand]->undo(*parent, scene, dt);
    currCommand--;
}

void InputComponent::redo(Scene& scene,  const float& dt)
{
    commandHistory[currCommand]->execute(*parent, scene, dt);
    currCommand++;
}
