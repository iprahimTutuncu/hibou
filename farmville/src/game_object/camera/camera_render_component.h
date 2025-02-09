#pragma once

#include <core/game_object/component/render_component.h>
#include <core/game_object/data/data.h>
#include <thsan/options.h>
#include <limits>

class CameraRenderComponent : public RenderComponent
{
    public:
        CameraRenderComponent(GameObject* player);

        void init(Scene& scene) override;

        ~CameraRenderComponent() = default;
    private:
        void onStart(Scene& scene, Thsan::RendererTerrain& renderer) override {};
        void onUpdate(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer) override;

        CameraData* cam_data;

        void setBeginAngle(CameraData* cd);
        void setEndAngle(glm::vec3 target_position, glm::vec3 player_position, float range_angle_degree, CameraData* cd);

        void setBeginHorizon(float horizon);
        void setEndHorizon(float horizon, Thsan::TerrainOptions& vt);

        void interpolateCamera(const float& dt);
        void interpolateCameraAngle(const float& dt);
        void interpolateCameraHorizon(const float& dt);

        GameObject* target;
        Scene* p_scene;
        std::vector<std::string> renderer_tags;
        PlayerData* player_data;
        glm::vec4 target_rect;

        const float CAMERA_ANGLE_INTERPOLATION_TIME = 0.5f;
        const float CAMERA_HORIZON_INTERPOLATION_TIME = 0.5f;

        float t_angle = 0.f;
        float t_horizon = 0.f;

        float begin_angle{ 0.f };
        float end_angle{ 0.f };

        float begin_horizon{ 0.f };
        float end_horizon{ 0.f };

        float* curr_angle{ nullptr };
        float* curr_horizon{ nullptr };

        float curr_offset{ 200.f };
        float speed = 20.f;

        float base_horizon{ std::numeric_limits<float>::max() };

        bool is_interpolating{ false };
        bool is_following{ false };

};