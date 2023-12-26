#pragma once
#include <iostream>
#include "thsan/Input/ControlSetting.h"
#include "thsan/graphics/render_manager.h"

class Game;

namespace Thsan {
    class THSAN_API State
    {
    public:
        State(Game* parent);

        virtual void init() = 0;

        virtual void input(const float& deltaTime, std::vector<InputAction> inputActions) = 0;
        virtual void update(const float& deltaTime) = 0;
        virtual void draw(RenderManager* target, const float& deltaTime) = 0;

        virtual ~State() {};

        Game* getParent() const { return parent; }

    protected:
        Game* parent;
    };

}