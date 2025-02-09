#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <functional>

class GameObject;
class Scene;

namespace Thsan
{
    enum InputAction;
    class RendererTerrain;
    class Renderer2D;
    class SpriteAnimation;
}

class RenderComponent
{
public:
    RenderComponent() = default;
    virtual ~RenderComponent() = default;

    virtual void init(Scene& scene){};
    virtual void start(Scene& scene, Thsan::RendererTerrain& renderer){};
    virtual void update(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer){};
    virtual void onDelete(Scene& scene) {};

    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }

protected:
    friend GameObject;
    friend Scene;

    GameObject* parent;
private:
    bool enabled{ true };
};

