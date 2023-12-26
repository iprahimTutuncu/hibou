#include "Transition.h"
#include "../../../ressourceManager/ShaderManager.h"

#include <SFML/Graphics/RenderTarget.hpp>

Transition::Transition(
	sf::Time time_in, sf::Time time_out, Type type_in, Type type_out, Fade_type fade_type_in, Fade_type fade_type_out, sf::Color in_color1, sf::Color in_color2, sf::Color out_color1, sf::Color out_color2):
	time_in(time_in),
	time_out(time_out),
	type_in(type_in),
	type_out(type_out),
	fade_type_in(fade_type_in),
	fade_type_out(fade_type_out),
	in_color1(in_color1),
	in_color2(in_color2),
	out_color1(out_color1),
	out_color2(out_color2)
{
	curr_time_in = sf::Time::Zero;
	curr_time_out = sf::Time::Zero;

	transition_shader = RessourceManager::ShaderManager::get("media/shader/transition/transition.vert", "media/shader/transition/transition.frag");

}

void Transition::setInputTexture(const sf::Texture texture)
{
	inputColorTexture = texture;
}

void Transition::update(sf::Time dt)
{
	if (isIn()) {
		curr_time_in += dt;
		if (curr_time_in >= time_in)
			isInDone = true;
	}
	else if (isOut()) {
		curr_time_out += dt;
		if (curr_time_out >= time_out)
			isOutDone = true;
	}

	sprite_output.setTexture(inputColorTexture);
}

bool Transition::isIn() const
{
	return !isInDone;
}

bool Transition::isOut() const
{
	return isInDone && !isOutDone;
}

bool Transition::isDone()
{
	return isInDone && isOutDone;
}

void Transition::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.shader = &*transition_shader;
    states.texture = &inputColorTexture;
    states.transform = getTransform();

    transition_shader->setUniform("width", static_cast<float>(inputColorTexture.getSize().x));
	transition_shader->setUniform("height", static_cast<float>(inputColorTexture.getSize().y));

	if (isIn()) {
		std::cout << "IN: " << curr_time_in.asSeconds() << "/" << time_in.asSeconds() << std::endl;
		transition_shader->setUniform("transition_t", curr_time_in.asSeconds() / time_in.asSeconds());
		transition_shader->setUniform("transition_type", static_cast<int>(type_in));
		transition_shader->setUniform("fading_type", static_cast<int>(fade_type_in));
		transition_shader->setUniform("transition_color1", sf::Glsl::Vec4(in_color1));
		transition_shader->setUniform("transition_color2", sf::Glsl::Vec4(in_color2));
	}
	if (isOut()) {

		std::cout << "OUT: " << curr_time_out.asSeconds() << "/" << time_out.asSeconds() << std::endl;
		transition_shader->setUniform("transition_t", 1.0f - (curr_time_out.asSeconds() / time_out.asSeconds()));
		transition_shader->setUniform("transition_type", static_cast<int>(type_out));
		transition_shader->setUniform("fading_type", static_cast<int>(fade_type_out));
		transition_shader->setUniform("transition_color1", sf::Glsl::Vec4(out_color1));
		transition_shader->setUniform("transition_color2", sf::Glsl::Vec4(out_color2));
	}

    target.draw(sprite_output, states);
}