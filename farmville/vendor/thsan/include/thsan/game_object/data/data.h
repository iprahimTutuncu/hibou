#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <unordered_map>

enum class DataType : unsigned int {
    NONE = 0,
    TRANSFORM = 1,
    UI = 2,
    TILE_MAP = 3,
    VOXEL_TERRAIN = 4,
    CAMERA = 5,
    HITBOX = 6,
    PHYSIC_BODY = 7,
    TREES = 8,
    PLAYER = 9,
    EDITABLE_SPRITE_ANIMATION = 10,
    EDITABLE_IMAGE = 11,
    LIGHT = 12,
    VOLUME_POST_PROCESS = 13,
    BARS = 14,
    PLANT = 15,
    GEAR = 16,
    CHARACTER_STAT = 17,
    BAG = 18
};


struct Data {};

struct Transform : public Data {
    Transform() {
        this->position = glm::vec3(0.f, 0.f, 0.f);
        this->scale = glm::vec2(1.f, 1.f);
        this->rotation = 0.f;
        this->parent = nullptr;
    }

    glm::vec3 position;
    glm::vec2 scale;
    float rotation;

    Transform* parent;
};


// note: position is in the center of the box.
//      In fact, every single position in shape
//      in hitBox is intended that way.

struct Sphere {
    float radius;
    glm::vec3 position;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 position;
};

struct Capsule {
    float radius;
    float height;
    glm::vec3 position;
};

enum class Collision_object {
    phantoms,
    real
};

struct HitBox : public Data {
    std::vector<Sphere> spheres;
    std::vector<AABB> AABBs;
    std::vector<Capsule> capsules;
    glm::vec3 position{ 0.f, 0.f, 0.f };
    Collision_object collision_object{ Collision_object::phantoms };
};



struct CameraData : public Data {
    bool isPolar{ false };
    std::string target_name{};
    glm::vec3 offset;
    glm::vec2 screen_position;
    glm::vec4 soft_zone;
    glm::vec4 dead_zone;
    glm::vec4 on_screen_target_rect;
    float offset_distance;
    float angle;
    float Damping;
};

struct physicBodyData : public Data {
    physicBodyData() {
        force = 0.f;
        friction = 0.99f;
        direction = glm::vec3(0.f, 0.f, 0.f);
        masse = 0.f;
    }
    float force;
    float friction;
    float masse;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    glm::vec3 direction;

    bool isJump{ false };
    bool isDoubleJump{ false };
    bool onGround{ true };

};

struct PlayerData : public Data {
    enum class State {
        idle,
        walk,
        run,
        attack,
        forward_attack,
        plant,
        damage,
        death,
        jump,
        double_jump,
        fall,
        land,
        charge
    };
    bool isAiming{ false };
    State curr_state;
    std::unordered_map<std::string, int> items;
    Transform* target_transform;
    std::string target_name;

    float atk;
    float def;
    float hp;
};

struct BarsData : public Data {
    bool show_bar{ false };
    float duration{ 0.5f };
    float end_speed{ 3.f }; // bars will hide at en_speed faster -> elapsed_time -= dt * end_speed
    float bar_screen_ratio_top{ 0.f };
    float bar_screen_ratio_bottom{ 0.f };
};

struct PlantData : public Data {
public:
    enum class Stage {
        seed = 0,
        sprout = 1,
        seeding = 2,
        vegetative = 3,
        budding = 4,
        flowering = 5,
        ripping = 6,
        LAST = 7
    };

    enum class State {
        planted = 0,
        eaten = 1,
        thrown = 2,
        wore = 3,
        physical_item = 4,
        inventory_item = 5,
        LAST = 6
    };

    PlantData() {
        growth_val[Stage::seed] = 0.f;
        growth_val[Stage::sprout] = 0.1f;
        growth_val[Stage::seeding] = 0.2f;
        growth_val[Stage::vegetative] = 0.3f;
        growth_val[Stage::budding] = 0.5f;
        growth_val[Stage::flowering] = 0.8f;
        growth_val[Stage::ripping] = 0.9f;

        static int id_increment = 0;
        name_id = "id__" + std::to_string(id_increment);
        id_increment++;
    }

    /*
        at: value of the point where the plant
            will become the specified stage. Usually
            Handled automatically by the plantPhysicComponent
     */
    void setGrowth(Stage stage, float at) {
        if (at > 1.0f)
            at = 1.0f;
        else if (at < 0.f)
            at = 0.f;

        growth_val[stage] = at;
    }

    float getGrowth(Stage stage) {
        return growth_val[stage];
    }

    std::string getId() const {
        return name_id;
    }

    Stage current_stage = Stage::seed;
    State current_state = State::inventory_item;
    std::string owner = "";

    float growth_time{ 5.f };
    std::string name = "";

    float hp{ 100 };

private:
    std::string name_id = "";
    std::unordered_map<Stage, float> growth_val;

};


struct CharacterStatData : public Data
{
    struct Current {
        int hp;
        int mp;
    };

    Current curr;

    int hp;
    int mp;
    int atk;
    int def;
    int speed;
};