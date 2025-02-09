#pragma once

#include "core/game_object/component/input/command.h"

class MoveLeftCommand : public Command
{
    public:
        MoveLeftCommand();
        ~MoveLeftCommand();

        void execute(GameObject &parent, Scene &scene, const float& dt) override;
        void undo(GameObject &parent, Scene &scene, const float& dt) override;
};
