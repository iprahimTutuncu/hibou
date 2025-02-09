#pragma once

#include "core/game_object/component/input/command.h"

class MoveRightCommand : public Command
{
    public:
        MoveRightCommand();
        ~MoveRightCommand();

        void execute(GameObject &parent, Scene &scene, const float& dt) override;
        void undo(GameObject &parent, Scene &scene, const float& dt) override;
};