#pragma once
#include <iostream>
#include "thsan/Input/control_setting.h"
#include "thsan/graphics/renderer/renderer_2D.h"
#include "thsan/graphics/renderer/renderer_terrain.h"
#include "thsan/graphics/renderer/renderer_film.h"

class Game;

namespace Thsan {
    class THSAN_API State
    {
    public:
        State(Game* parent);

        // onInit will be called before most game element, for exemple the renderer
        virtual void onInit() = 0;
        virtual void onStart() = 0;
        virtual void onExit() = 0;
        virtual void onSuspend() = 0;
        virtual void onResume() = 0;

        virtual void onInput(const float& deltaTime, std::vector<InputAction> inputActions) = 0;
        virtual void onUpdate(const float& deltaTime) = 0;
        virtual void onDraw(Renderer2D& target, const float& deltaTime) = 0;
        virtual void onDraw(RendererTerrain& target, const float& deltaTime) = 0;


        virtual ~State() {};

        Game* getParent() const { return parent; }

    protected:
        Game* parent;
    };
}