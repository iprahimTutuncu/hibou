#pragma once

#include "core/game_object/component/input/command.h"

class MoveUpCommand : public Command
{
    public:
        MoveUpCommand();
        ~MoveUpCommand();

        void execute(GameObject &parent, Scene &scene, const float& dt) override;
        void undo(GameObject &parent, Scene &scene, const float& dt) override;
};
