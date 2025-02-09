#pragma once
#include "core/game_object/component/input/command.h"
#include "states/postProcess/blackBar/BlackBarPostProcessState.h"

class AimCommand : public Command
{
public:
    AimCommand();
    ~AimCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:
    bool first_aim{true};
    void show_bar();
    void hide_bar();
    std::shared_ptr<PostProcess> postProcess;
};