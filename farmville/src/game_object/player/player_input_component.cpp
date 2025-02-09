#include "player_input_component.h"
#include <thsan/state/state_manager.h>
#include <core/game_object/data/data.h>
#include <core/game_object/game_object.h>
#include <iostream>

// Include command headers
#include "command/move_down_command.h"
#include "command/move_left_command.h"
#include "command/move_right_command.h"
#include "command/move_up_command.h"
#include "command/jump_command.h"
#include "command/rotate_left_command.h"
#include "command/rotate_right_command.h"
#include "command/run_command.h"
#include "command/stop_run_command.h"

PlayerInputComponent::PlayerInputComponent()
    : left(std::make_unique<MoveLeftCommand>()),
    down(std::make_unique<MoveDownCommand>()),
    up(std::make_unique<MoveUpCommand>()),
    right(std::make_unique<MoveRightCommand>()),
    jump(std::make_unique<JumpCommand>()),
    rotateLeft(std::make_unique<RotateLeftCommand>()),
    rotateRight(std::make_unique<RotateRightCommand>()),
    run(std::make_unique<RunCommand>()),
    stop_run(std::make_unique<StopRunCommand>())
{
}

void PlayerInputComponent::init(Scene& scene)
{
    if (!parent->hasComponent(DataType::PLAYER)) {
        player_data = new PlayerData();
        parent->setComponent(DataType::PLAYER, player_data);
    }
    else {
        player_data = parent->getComponent<PlayerData>(DataType::PLAYER);
    }

    if (!parent->hasComponent(DataType::BAG)) {
        bag_data = new BagData();
        parent->setComponent(DataType::BAG, bag_data);
    }
    else {
        bag_data = parent->getComponent<BagData>(DataType::BAG);
    }

    if (!parent->hasComponent(DataType::CHARACTER_STAT)) {
        character_stat_data = new CharacterStatData();
        parent->setComponent(DataType::CHARACTER_STAT, character_stat_data);
    }
    else {
        character_stat_data = parent->getComponent<CharacterStatData>(DataType::CHARACTER_STAT); // Corrected to CHARACTER_STAT
    }

    player_data->curr_state = PlayerData::State::idle;

    // Load from BD...DTO
    character_stat_data->hp = 100;
    character_stat_data->mp = 10;
    character_stat_data->curr.hp = 75;
    character_stat_data->curr.mp = 5;
}

void PlayerInputComponent::update(Scene& scene, const float& dt, std::vector<Thsan::InputAction> inputActions)
{
    PhysicBodyData* phy = parent->getComponent<PhysicBodyData>(DataType::PHYSIC_BODY);

    if (phy)
        phy->direction = glm::vec3(0, 0, 0);

    std::vector<Command*> commands = handleInput(inputActions);

    for (auto c : commands) {
        c->execute(*parent, scene, dt);
        historyInput.push_back(c);
    }

    commands.clear();
}

std::vector<Command*> PlayerInputComponent::handleInput(std::vector<Thsan::InputAction> inputActions)
{
    std::vector<Command*> commands;

    for (Thsan::InputAction inputAction : inputActions) {
        if (inputAction == Thsan::InputAction::up)
            commands.push_back(up.get());
        if (inputAction == Thsan::InputAction::down)
            commands.push_back(down.get());
        if (inputAction == Thsan::InputAction::left)
            commands.push_back(left.get());
        if (inputAction == Thsan::InputAction::right)
            commands.push_back(right.get());
        if (inputAction == Thsan::InputAction::jump)
            commands.push_back(jump.get());
        if (inputAction == Thsan::InputAction::rotateLeft)
            commands.push_back(rotateLeft.get());
        if (inputAction == Thsan::InputAction::rotateRight)
            commands.push_back(rotateRight.get());
        if (inputAction == Thsan::InputAction::run)
            commands.push_back(run.get());
        if (inputAction == Thsan::InputAction::stop_run)
            commands.push_back(stop_run.get());
    }

    return commands;
}
