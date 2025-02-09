#pragma once

#include "core/game_object/component/input/command.h"
#include "../../plant/PlantFactory.h"

class PlantCommand : public Command
{
public:
    PlantCommand();
    ~PlantCommand() = default;

    void execute(GameObject& parent, Scene& scene, const float& dt) override;
    void undo(GameObject& parent, Scene& scene, const float& dt) override;
private:
    PlantFactory* plantFactory;
    PlayerData* data;
};