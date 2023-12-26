#pragma once

#include "../../../graphic/PostProcess.h"
#include <iostream>
#include <functional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Transition : public sf::Drawable, sf::Transformable
{
public:
    enum class Type {
        fade_to_color = 0,
        left_to_right = 1,
        right_to_left = 2,
        left_right_to_center = 3,
        top_to_bottom = 4,
        bottom_to_top = 5,
        top_bottom_to_center = 6,
        zoom_blur_to_color = 7,
        LAST = 8
    };

    enum class Fade_type {
        step = 0,
        smoothstep = 1,
        LAST = 2
    };

    Transition(
        sf::Time time_in,
        sf::Time time_out,
        Type type_in,
        Type type_out,
        Fade_type fade_type_in = Fade_type::step,
        Fade_type fade_type_out = Fade_type::step,
        sf::Color in_color1 = sf::Color::Black,
        sf::Color in_color2 = sf::Color::Black,
        sf::Color out_color1 = sf::Color::Black,
        sf::Color out_color2 = sf::Color::Black
    );

    ~Transition() = default;
    Transition(const Transition&) = default;

    void setInputTexture(const sf::Texture texture);
    void update(sf::Time dt);

    bool isIn() const;
    bool isOut() const;
    bool isDone();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool isInDone{ false };
    bool isOutDone{ false };

    sf::Time curr_time_in;
    sf::Time curr_time_out;

    sf::Time time_in;
    sf::Time time_out;

    sf::Color in_color1;
    sf::Color in_color2;
    sf::Color out_color1;
    sf::Color out_color2;

    Fade_type fade_type_in;
    Fade_type fade_type_out;

    Type type_in;
    Type type_out;

    sf::Texture inputColorTexture;

    sf::Sprite sprite_output;

    std::shared_ptr<sf::Shader> transition_shader;
};
