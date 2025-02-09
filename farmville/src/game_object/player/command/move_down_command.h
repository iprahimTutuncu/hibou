#pragma once

#include "core/game_object/component/input/command.h"

class MoveDownCommand : public Command
{
    public:
        MoveDownCommand();
        ~MoveDownCommand();

        void execute(GameObject &parent, Scene &scene, const float& dt) override;
        void undo(GameObject &parent, Scene &scene, const float& dt) override;
};
