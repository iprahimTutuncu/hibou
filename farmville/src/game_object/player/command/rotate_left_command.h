#pragma once

#include "core/game_object/component/input/command.h"

class RotateLeftCommand : public Command
{
public:
    RotateLeftCommand();
    ~RotateLeftCommand();

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
};
