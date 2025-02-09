#pragma once

#include "core/game_object/component/input/command.h"

class RunCommand : public Command
{
public:
    RunCommand();
    ~RunCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:

};