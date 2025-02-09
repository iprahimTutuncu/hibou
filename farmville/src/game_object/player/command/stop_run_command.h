#pragma once

#include "core/game_object/component/input/command.h"

class StopRunCommand : public Command
{
public:
    StopRunCommand();
    ~StopRunCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:

};