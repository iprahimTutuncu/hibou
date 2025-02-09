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

    virtual void init(Scene& scene) {};
    virtual void onDelete(Scene& scene) {};

    void update(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer)
    {
        static bool OnStart = true;
        if (OnStart) {
            OnStart = false;
            onStart(scene, renderer);
        }
        onUpdate(scene, dt, renderer);
        //m_renderComponent->callEvent( scene, delta);
    }

    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }

protected:
    virtual void onStart(Scene& scene, Thsan::RendererTerrain& renderer) {};
    virtual void onUpdate(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer) {};

    friend GameObject;
    friend Scene;

    GameObject* parent;
private:
    bool enabled{ true };
};