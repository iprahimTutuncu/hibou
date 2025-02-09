#pragma once

#include <core/game_object/component/input_component.h>
#include <memory>

struct PlayerData;
struct BagData;
struct CharacterStatData;

class Command;

class PlayerInputComponent : public InputComponent
{
    public:
        PlayerInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const float& dt, std::vector<Thsan::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Thsan::InputAction> inputActions) override;

        ~PlayerInputComponent() = default;

    private:
        std::unique_ptr<Command> left;
        std::unique_ptr<Command> down;
        std::unique_ptr<Command> up;
        std::unique_ptr<Command> right;
        std::unique_ptr<Command> plant; //todo
        std::unique_ptr<Command> jump;
        std::unique_ptr<Command> aim; //todo
        std::unique_ptr<Command> stop_aim; //todo
        std::unique_ptr<Command> rotateLeft;
        std::unique_ptr<Command> rotateRight;
        std::unique_ptr<Command> run;
        std::unique_ptr<Command> stop_run;
        std::unique_ptr<Command> action; //todo
        std::unique_ptr<Command> attack; //todo
        std::vector<Command*> historyInput;

        PlayerData* player_data;
        BagData* bag_data;
        CharacterStatData* character_stat_data;
};

