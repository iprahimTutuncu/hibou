#pragma once

#include <memory>
#include <thsan/core.h>

class GameObject;
class Scene;

class PhysicComponent
{
public:
    virtual ~PhysicComponent() = default;

    virtual void init(Scene& scene) {};
    virtual void update(Scene& scene, const float& dt) {};
    virtual void onDelete(Scene& scene) {};

    bool isActive() const { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }

protected:
    GameObject* parent;
    friend class GameObject;
    friend class Scene;

private:
    bool enabled{ true };
};
