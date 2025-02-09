#pragma once
#include <thsan/Input/Command.h>
#include "../../../states/postProcess/blackBar/BlackBarPostProcessState.h"

class StopAimCommand : public Command
{
public:
    StopAimCommand();
    ~StopAimCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:
    
};

