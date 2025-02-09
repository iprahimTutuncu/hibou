#pragma once
#include <vector>

class GameObject;
class Scene;
class Command;

namespace Thsan
{
    enum InputAction;
}

class InputComponent
{
public:
    InputComponent() {};
    virtual void init(Scene& scene) {}
    virtual void update(Scene& scene, const float& dt, std::vector<Thsan::InputAction> inputActions) {}
    virtual void onDelete(Scene& scene) {};
    virtual std::vector<Command*> handleInput(std::vector<Thsan::InputAction> inputActions) { return std::vector<Command*>(); }
    virtual ~InputComponent();

    void undo(Scene& scene, const float& dt);
    void redo(Scene& scene, const float& dt);
    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }
protected:
    GameObject* parent;
    std::vector<Command*> commandHistory;
    int currCommand{ 0 };

    friend Scene;
    friend GameObject;
private:
    bool enabled{ true };
};