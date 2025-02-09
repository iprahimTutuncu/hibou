#pragma once
#include <thsan/graphics/post_process.h>

namespace Thsan 
{
    class LetterboxPostProcessor : public PostProcessor
    {
    public:
        void setEffectParam(const EffectParams params) override;

        void apply(std::weak_ptr<Texture2D> out, float opacity) override;

        void setUp() override;
        void cleanUp() override;

        Effect GetEffectType() const override { return Effect::Letterbox; }
    };
}