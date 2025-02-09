#pragma once

class GameObject;
class Scene;

class PhysicComponent
{
    public:
        PhysicComponent() = default;

        virtual void init(Scene& scene){};
        virtual void update(Scene& scene, const float& dt){};
        virtual void onDelete(Scene& scene) {};

        virtual ~PhysicComponent() = default;

        bool isActive() { return enabled; }
        void disable() { enabled = false; }
        void enable() { enabled = true; }
protected:
    GameObject* parent;
    friend Scene;
    friend GameObject;
private:
    bool enabled{ true };
};

