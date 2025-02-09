#include "camera_render_component.h"

#include "core/game_object/data/data.h"
#include "core/game_object/game_object.h"
#include "core/scene.h"

#include <glm/gtx/easing.hpp>
#include <iostream>
#include <thsan/game.h>
#include <thsan/state/state.h>
#include <thsan/graphics/renderer/renderer_terrain.h>
#include <thsan/graphics/drawable/sprite_animation.h>

CameraRenderComponent::CameraRenderComponent(GameObject* player):
	target(player)
{

}

void CameraRenderComponent::init(Scene& scene)
{
	if (!parent->hasComponent(DataType::CAMERA))
		parent->setComponent(DataType::CAMERA, new CameraData());

	cam_data = parent->getComponent<CameraData>(DataType::CAMERA);

	p_scene = &scene;

	if (!parent->hasComponent(DataType::CAMERA))
		parent->setComponent(DataType::CAMERA, new CameraData());

	cam_data->target_name = target->getName();

	cam_data->offset.y = 100.f;
	cam_data->offset.z = 100.f;
	cam_data->angle = 0.f;
	cam_data->offset_distance = 50.f;
	cam_data->isPolar = true;

	curr_offset = cam_data->offset_distance;
}

void CameraRenderComponent::onUpdate(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer)
{
	renderer_tags = renderer.getAllTags();

	PlayerData* player_data = scene.get_child("player")->getComponent<PlayerData>(DataType::PLAYER);
	glm::vec3 player_position = scene.get_child("player")->getComponent<Transform>(DataType::TRANSFORM)->position;
	float middle = scene.getParent()->getParent()->options.windowOptions.screenHeight / 2.f;
	
	CameraData* cd = parent->getComponent<CameraData>(DataType::CAMERA);
	Thsan::TerrainOptions* vt = &scene.getParent()->getParent()->options.renderOptions.terrainOptions;
	Thsan::WindowOption* wt = &scene.getParent()->getParent()->options.windowOptions;
	Transform* player_transform = scene.get_child("player")->getComponent<Transform>(DataType::TRANSFORM);
	Transform* transform = parent->getComponent<Transform>(DataType::TRANSFORM);

	glm::vec3 position = player_transform->position;

	cd->offset_distance = 50.f + position.y / 8.f;

	if (vt) {

		if (base_horizon == (std::numeric_limits<float>::max)())
			base_horizon = vt->horizon;

		if (target) {
			if (cd->target_name != target->getName()) {
				target = scene.get_child(cd->target_name);
			}
			if (target) {

				vt->angle = cd->angle;
				curr_offset = cd->offset_distance;
				float x = curr_offset * sin(cd->angle);
				float z = curr_offset * cos(cd->angle);

				glm::vec3 target_position = target->getComponent<Transform>(DataType::TRANSFORM)->position;
				glm::vec3 camera_position = target_position + glm::vec3(x, cd->offset.y, z);

				float height{ 0.f };
				/*
				do {

					height = scene.getPhysicalHeightOnPosition(sf::Vector2f(camera_position.x, camera_position.z));
					height -= camera_position.y;
					if (height > 0.f) {
						if (height < 20.f)
							camera_position.y += height / 2.f;

						curr_offset -= 1.f;
						x = curr_offset * sin(cd->angle);
						z = curr_offset * cos(cd->angle);
						camera_position = target_position + glm::vec3(x, cd->offset.y, z);
					}
				} while (height > 0.f);
				*/

			}
		}
		else
			target = scene.get_child(cd->target_name);

		interpolateCamera(dt);

		/*
		*	aiming if target is a player and wants to aim his own target
		*/

		renderer_tags = renderer.getAllTags();


		glm::vec3 player_position;
		float middle = wt->screenHeight / 2.f;

		if (target->hasComponent(DataType::PLAYER)) {
			player_data = target->getComponent<PlayerData>(DataType::PLAYER);
			player_position = target->getComponent<Transform>(DataType::TRANSFORM)->position;
		}
		else
			player_data = nullptr;

		if (player_data) {

			if (player_data->target_name == "") {
				t_angle = 0.f;
				t_horizon = 0.f;
				curr_angle = nullptr;
				curr_horizon = nullptr;
			}


			for (const auto& tag : renderer_tags) {

				if (tag == player_data->target_name) {
					is_following = true;
					glm::vec3 target_position = scene.get_child(player_data->target_name)->getComponent<Transform>(DataType::TRANSFORM)->position;
					if (!curr_angle || t_angle >= CAMERA_ANGLE_INTERPOLATION_TIME) {
						setBeginAngle(cd);
					}

					if (!curr_horizon || t_horizon >= CAMERA_HORIZON_INTERPOLATION_TIME) {
						setBeginHorizon(vt->horizon);
					}

					setEndAngle(target_position, player_position, 50.f, cd);

					auto player = renderer.get(target->getName());
					auto target = renderer.get(player_data->target_name);

					glm::vec2 player_screen_position = player->getPosition();
					glm::vec2 target_screen_position = target->getPosition();

					float player_height = player->getFrameHeight();
					float target_height = target->getFrameHeight();

					// Calculate midpoints
					float player_midpoint_y = player_screen_position.y + (player_height / 2.f);
					float target_midpoint_y = target_screen_position.y + (target_height / 2.f);

					// Calculate total screen height
					float screen_height = player_midpoint_y + target_midpoint_y;

					screen_height /= 2.f;
					setEndHorizon(middle + (middle - screen_height / 2), *vt);
				}

			}
		}


	}
}

void CameraRenderComponent::interpolateCamera(const float& dt)
{
	interpolateCameraAngle(dt);
	interpolateCameraHorizon(dt);
}

void CameraRenderComponent::interpolateCameraAngle(const float& dt)
{
	if (!curr_angle)
		return;

	t_angle += dt;

	if (t_angle > CAMERA_ANGLE_INTERPOLATION_TIME) {
		t_angle = CAMERA_ANGLE_INTERPOLATION_TIME;
		curr_angle = nullptr;
	}
	float s = glm::cubicEaseOut<float>(t_angle / CAMERA_ANGLE_INTERPOLATION_TIME);
	if (curr_angle)
		*curr_angle = begin_angle * (1.f - s) + end_angle * s;

}

void CameraRenderComponent::interpolateCameraHorizon(const float& dt)
{
	if (!curr_horizon)
		return;

	t_horizon += dt;
	if (t_horizon > CAMERA_HORIZON_INTERPOLATION_TIME) {
		t_horizon = CAMERA_HORIZON_INTERPOLATION_TIME;
		curr_horizon = nullptr;
	}
	float s = glm::cubicEaseOut<float>(t_horizon / CAMERA_HORIZON_INTERPOLATION_TIME);
	if (curr_horizon)
		*curr_horizon = begin_horizon * (1.f - s) + end_horizon * s;

}


void CameraRenderComponent::setBeginAngle(CameraData* cd)
{
	begin_angle = cd->angle;
}

void CameraRenderComponent::setEndAngle(glm::vec3 target_position, glm::vec3 player_position, float range_angle_degree, CameraData* cd)
{
	glm::vec2 n = glm::vec2(cos(begin_angle), sin(begin_angle));

	//X axis of device coordinate
	glm::vec2 m = glm::vec2(
		target_position.z - player_position.z,
		target_position.x - player_position.x
	);

	m = -m;

	//get smallest angle
	float angle = atan2(m.x, m.y) - atan2(n.x, n.y);

	if (angle > glm::pi<float>())
		angle = -glm::two_pi<float>() + angle;
	else if (angle < -glm::pi<float>()) {
		angle = glm::two_pi<float>() + angle;
	}

	//get to know if it move left or right
	float direction = 1.0f;
	angle < 0.f ? direction = -1.0f : direction = 1.0f;

	float range_angle = (range_angle_degree * glm::pi<float>() / 180.f);
	//set end angle
	if (glm::abs(angle) > range_angle)
		end_angle = (begin_angle + direction * range_angle) - angle;
	else
		end_angle = begin_angle;

	if (!curr_angle)
		curr_angle = &cd->angle;
}

void CameraRenderComponent::setBeginHorizon(float horizon)
{
	begin_horizon = horizon;
}

void CameraRenderComponent::setEndHorizon(float horizon, Thsan::TerrainOptions& vt)
{
	end_horizon = horizon;

	if (!curr_horizon)
		curr_horizon = &vt.horizon;
}


