#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <future>
#include <thsan/log.h>
#include <thsan/state/state.h>
#include "default_state.h"

namespace Thsan {

    class Game;

    class StateManager {
    public:
        StateManager():defaultState(nullptr){}
        ~StateManager() 
        {
            for (auto& pair : states)
                pair.second.reset();
            states.clear();
        }

        template <typename T, typename ...Args>
        void registerState(const std::string& name, Args&& ...args) {
            std::promise<std::unique_ptr<T>> promise;
            std::future<std::unique_ptr<T>> future = promise.get_future();

            std::thread([this, name, args..., promise = std::move(promise)]() mutable {
                TS_CORE_TRACE("registering {}...", name);
                std::unique_ptr<T> state = std::make_unique<T>(gameParent, std::forward<Args>(args)...);
                promise.set_value(std::move(state));
                }).detach();

                // Wait for the promise to be fulfilled and assign the state
                std::thread([this, name, future = std::move(future)]() mutable {
                    std::unique_ptr<T> result = future.get();
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        result->onInit();
                        TS_CORE_TRACE("Initialized state {}.", name);
                        states[name] = std::move(result);
                        currentState = name;
                    }

                    TS_CORE_TRACE("DONE registering {}.", name);

                }).detach();
        }

        bool isStateRegistered(const std::string& name)
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = states.find(name);
            return it != states.end();
        }
        void changeState(const std::string& name);
        void removeState(const std::string& name) { /* remove it but you may need to call a bunch of shit */ };

    private:
        friend class Game;
        void init(Game* game);
        static StateManager* create();
        State& getCurrentState();
        DefaultState defaultState;
        std::unordered_map<std::string, std::unique_ptr<Thsan::State>> states;
        std::string currentState{""};
        std::mutex mtx;

        Game* gameParent;
    };
}