#pragma once

#include <glm/glm.hpp>
#include <thsan/core.h>
#include <array>
#include <vector>
#include <memory>
#include <variant>
#include <iostream>

namespace Thsan {

    class ComputeShader;
    class Texture2D;

    enum class THSAN_API Effect {
        Snowfall,
        Letterbox,
        MAX_EFFECT // Placeholder for max effects
    };

    struct THSAN_API Vec2
    {
        float x, y;
    };

    struct THSAN_API Color
    {
        float r, g, b, a;
    };

    struct THSAN_API SnowfallParam {
        Vec2 wind_direction;
        Vec2 speed;
        float size_rate;
        float fg_layers;
        float bg_layers;
        float layer_gap;
    };

    struct THSAN_API LetterboxParam {
        enum class Type { Step, Fade };

        enum class Bar { Top, Bottom, Right, Left };

        Type type;
        std::array<Bar, 4> priority_letterbox;
        int blend;
        Color top_bar_color;
        Color bottom_bar_color;
        Color left_bar_color;
        Color right_bar_color;
        float bar_screen_ratio_top;
        float bar_screen_ratio_bottom;
        float bar_screen_ratio_left;
        float bar_screen_ratio_right;
        float fading_range_top;
        float fading_range_bottom;
        float fading_range_left;
        float fading_range_right;
    };

    using EffectParams = std::variant<SnowfallParam, LetterboxParam>;

    namespace Utils {
        inline glm::vec2 ToGlmVec2(const Vec2& v) {
            return glm::vec2(v.x, v.y);
        }

        inline glm::vec4 ToGlmVec4(const Color& c) {
            return glm::vec4(c.r, c.g, c.b, c.a);
        }
    }

    class THSAN_API PostProcessor 
    {
    public:
        PostProcessor() = default;
        virtual ~PostProcessor() = default;
        virtual void setEffectParam( const EffectParams params ) = 0;
        virtual void apply(std::weak_ptr<Texture2D> out, float opacity) = 0;

        virtual Effect GetEffectType() const = 0;

    protected:
        //use those for managing the computer shader
        virtual void setUp() = 0;
        virtual void cleanUp() = 0;
    };


    class THSAN_API PostProcessorManager {
    public:
        virtual ~PostProcessorManager() = default;

        virtual void add(std::shared_ptr<PostProcessor> processor, float opacity) = 0;

        virtual void display() = 0;

        virtual void clear() = 0;

        virtual void setProcessorOrder(const std::vector<size_t>& order) = 0;

        virtual std::shared_ptr<Texture2D> getOutTexture() = 0;

        static std::unique_ptr<PostProcessorManager> create();

        static std::shared_ptr<PostProcessor> createSnowFallPostProcress();
        static std::shared_ptr<PostProcessor> createLetterBoxPostProcress();
    };

    class PostProcessorManagerImpl : public PostProcessorManager {
    public:
        void add(std::shared_ptr<PostProcessor> processor, float opacity) override;
        void display() override;
        void clear() override;
        void setProcessorOrder(const std::vector<size_t>& order) override;

        virtual std::shared_ptr<Texture2D> getOutTexture() override;

    private:
        std::shared_ptr<Texture2D> out;
        std::vector<std::pair<std::shared_ptr<PostProcessor>, float>> processors_; 
    };
}
