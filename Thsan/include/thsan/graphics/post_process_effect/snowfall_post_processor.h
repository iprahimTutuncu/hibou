#pragma once
#include <thsan/graphics/post_process.h>



namespace Thsan {
    class SnowfallPostProcessor : public PostProcessor {
    public:
        void setEffectParam(const EffectParams params) override;

        void apply(std::weak_ptr<Texture2D> out, float opacity) override;

        Effect GetEffectType() const override{ return Effect::Snowfall;}
    private:
        //add compute shader
        void setUp() override;
        void cleanUp() override;
    };
}