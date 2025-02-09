#include "pch.h"
#include "thsan/state/state_manager.h"
#include <thsan/log.h>

namespace Thsan {

    void StateManager::changeState(const std::string& name) {
        if (isStateRegistered(name)) {
            if (currentState != "")
                states[currentState]->onExit();
            currentState = name;
            states[currentState]->onStart();
        }
        else {
            TS_CORE_WARN("State {} not found!", name);
        }
    }

    void StateManager::init(Game* game)
    {
        gameParent = game;
    }

    StateManager* StateManager::create()
    {
        return new StateManager();
    }

    State& StateManager::getCurrentState()
    {
        if (currentState != "")
            return *states[currentState];
        else {
            TS_CORE_WARN("StateManager::getCurrentState() is returning the defaultState, this is not a good sign.");
            return defaultState;
        }
    }



} // namespace Thsan
