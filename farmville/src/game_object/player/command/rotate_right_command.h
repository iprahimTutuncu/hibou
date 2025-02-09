#pragma once

#include <core/game_object/component/input/command.h>

class RotateRightCommand : public Command
{
public:
    RotateRightCommand();
    ~RotateRightCommand();

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
};