#pragma once

#include <iostream>
#include <thread>
#include <string>
#include "transition/Transition.h"
#include "../State.h"

template <class T>
class TransitionState : public State
{
public:
     
    template< typename... Args>
    TransitionState(Game* parent, std::shared_ptr<State> current, std::unique_ptr<Transition>& transition_ptr, Args... args):
        State(parent),
        curr(current)
    {
        state_construct_func = [this, args...]() {
            return std::make_shared<T>(this->getParent(), args...);
        };

        this->transition_ptr = std::move(transition_ptr);
        render_texture.create(m_parent->getWidth(), m_parent->getHeight());
    }

    TransitionState(Game* parent, std::shared_ptr<State> current, std::shared_ptr<State> next, std::unique_ptr<Transition>& transition_ptr) :
        State(parent),
        curr(current),
        next(next)
    {
        state_construct_func = []() {
            return nullptr;
        };

        this->transition_ptr = std::move(transition_ptr);
        render_texture.create(m_parent->getWidth(), m_parent->getHeight());
    }
    
    std::string getType() override 
    {
        return "transition";
    }

    void init() override
    {
        loader = std::thread(&TransitionState::load, this);
    }


    void handleEvent(sf::Event event) override
    {
    }

    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override
    {
        if (transition_ptr->isOut() && isJoined) {
            next->input(deltaTime, inputActions);
        }
    }

    void update(const sf::Time& deltaTime)
    {

        if (transition_ptr->isIn()) {

            curr->draw(render_texture, deltaTime);
            render_texture.display();
            first_in = false;
            
            transition_ptr->setInputTexture(render_texture.getTexture());
            transition_ptr->update(deltaTime);
        }

        else if (transition_ptr->isOut() && isJoined) {
            next->draw(render_texture, deltaTime);
            next->update(deltaTime);
            render_texture.display();
            
            transition_ptr->setInputTexture(render_texture.getTexture());
            transition_ptr->update(deltaTime);
        }

        if (transition_ptr->isDone()) {
            std::cout << "scene loaded..." << next << std::endl;
            this->getParent()->changeState(next, false);
        }

        if (isLoaded && !isJoined && loader.joinable()) {
            loader.join();
            isJoined = true;
        }

        
    }

    void draw(sf::RenderTarget& target, const sf::Time& deltaTime)
    {
       target.draw(*transition_ptr.get());
    }

    ~TransitionState()
    {
        if (loader.joinable())
            loader.join();
    }

private:

    void load()
    {
     
        std::cout << "calling constructor of next state..\n";
        std::shared_ptr<State> state_construct_result;
        state_construct_result = state_construct_func();
        
        if (!state_construct_result) {
            std::cout << "state already loaded..." << std::endl;
            isLoaded = true;
            return;
        }
        else
            next = state_construct_result;

        std::cout << "init scene..." << std::endl;

        if (curr != next) {
            next->init();
            next->update(sf::Time::Zero);
            sf::RenderTexture temp_rt;
            temp_rt.create(1, 1);
            next->draw(temp_rt, sf::Time::Zero);
        }

        isLoaded = true;
        std::cout << "init done..." << next << std::endl;
        
    }
    std::function<std::shared_ptr<State>()> state_construct_func;

    std::thread loader;
    std::shared_ptr<State> curr;
    std::shared_ptr<State> next;

    std::unique_ptr<Transition> transition_ptr;
    sf::RenderTexture render_texture;

    bool isLoaded{ false };
    bool isJoined{ false };

    bool first_in{ true };
};

