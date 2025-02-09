#include "state.h"
#include <thsan/log.h>

namespace Thsan {
    class DefaultState : public State {
    public:
        DefaultState(Game* parent) : State(parent) {}

        void onInit() override {
            TS_CORE_TRACE("DefaultState::onInit() method called");
        }

        void onStart() override {
            TS_CORE_TRACE("DefaultState::onStart() method called");
        }

        void onExit() override
        {
            TS_CORE_TRACE("DefaultState::onExit() method called");
        }

        void onSuspend() override 
        {
            TS_CORE_TRACE("DefaultState::onSuspend() method called");
        }

        void onResume() override 
        {
            TS_CORE_TRACE("DefaultState::onResume() method called");
        }

        void onInput(const float& deltaTime, std::vector<InputAction> inputActions) override
        {
            TS_CORE_TRACE("DefaultState::onInput() method called");
        }

        void onUpdate(const float& deltaTime) override 
        {
            TS_CORE_TRACE("DefaultState::onUpdate() method called");
        }

        void onDraw(Renderer2D& target, const float& deltaTime) override 
        {
            TS_CORE_TRACE("DefaultState::onDraw() method called");
        }

        void onDraw(RendererTerrain& target, const float& deltaTime) override
        {
            TS_CORE_TRACE("DefaultState::onDraw() method called");
        }
    };
}