#pragma once

#include "core/game_object/component/input/command.h"

class JumpCommand : public Command
{
public:
    JumpCommand();
    ~JumpCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:
    bool isJumping;
};